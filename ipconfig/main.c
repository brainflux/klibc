/*
 * ipconfig/main.c
 */
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>

#include "netdev.h"
#include "bootp_packet.h"
#include "bootp_proto.h"
#include "dhcp_proto.h"
#include "packet.h"

static const char *progname;

struct state {
	int		state;
	int		restart_state;
	time_t		expire;
	int		retry_period;

	struct netdev	*dev;
	struct state	*next;
};

static inline const char *my_inet_ntoa(__u32 addr)
{
	struct in_addr a;

	a.s_addr = addr;

	return inet_ntoa(a);
}

static void print_device_config(struct netdev *dev)
{
	printf("IP-Config: %s complete (from %s):\n", dev->name,
		my_inet_ntoa(dev->serverid ? dev->serverid : dev->ip_server));
	printf(" address: %-16s ", my_inet_ntoa(dev->ip_addr));
	printf("broadcast: %-16s ", my_inet_ntoa(dev->ip_broadcast));
	printf("netmask: %-16s\n", my_inet_ntoa(dev->ip_netmask));
	printf(" gateway: %-16s ", my_inet_ntoa(dev->ip_gateway));
	printf("dns0     : %-16s ", my_inet_ntoa(dev->ip_nameserver[0]));
	printf("dns1   : %-16s\n", my_inet_ntoa(dev->ip_nameserver[1]));
	if (dev->hostname[0])
		printf(" host   : %-64s\n", dev->hostname);
	if (dev->dnsdomainname[0])
		printf(" domain : %-64s\n", dev->dnsdomainname);
	if (dev->nisdomainname[0])
		printf(" nisdomain: %-64s\n", dev->nisdomainname);
	printf(" rootserver: %s ", my_inet_ntoa(dev->ip_server));
	printf("rootpath: %s\n", dev->bootpath);
}

static void configure_device(struct netdev *dev)
{
	if (netdev_setaddress(dev))
		printf("IP-Config: failed to set addresses on %s\n", dev->name);
	if (netdev_setdefaultroute(dev))
		printf("IP-Config: failed to set default route on %s\n", dev->name);
}

static void dump_device_config(struct netdev *dev)
{
	char fn[40];
	FILE *f;

	snprintf(fn, sizeof(fn), "/tmp/net-%s.conf", dev->name);
	f = fopen(fn, "w");
	if (f) {
		fprintf(f, "DEVICE=%s\n", dev->name);
		fprintf(f, "IPV4ADDR=%s\n", my_inet_ntoa(dev->ip_addr));
		fprintf(f, "IPV4BROADCAST=%s\n", my_inet_ntoa(dev->ip_broadcast));
		fprintf(f, "IPV4NETMASK=%s\n", my_inet_ntoa(dev->ip_netmask));
		fprintf(f, "IPV4GATEWAY=%s\n", my_inet_ntoa(dev->ip_gateway));
		fprintf(f, "IPV4DNS0=%s\n", my_inet_ntoa(dev->ip_nameserver[0]));
		fprintf(f, "IPV4DNS1=%s\n", my_inet_ntoa(dev->ip_nameserver[1]));
		fprintf(f, "HOSTNAME=%s\n", dev->hostname);
		fprintf(f, "DNSDOMAIN=%s\n", dev->dnsdomainname);
		fprintf(f, "NISDOMAIN=%s\n", dev->nisdomainname);
		fprintf(f, "ROOTSERVER=%s\n", my_inet_ntoa(dev->ip_server));
		fprintf(f, "ROOTPATH=%s\n", dev->bootpath);
		fclose(f);
	}
}

static __u32 inet_class_netmask(__u32 ip)
{
	ip = ntohl(ip);
	if (IN_CLASSA(ip))
		return htonl(IN_CLASSA_NET);
	if (IN_CLASSB(ip))
		return htonl(IN_CLASSB_NET);
	if (IN_CLASSC(ip))
		return htonl(IN_CLASSC_NET);
	return INADDR_ANY;
}

static void postprocess_device(struct netdev *dev)
{
	if (dev->ip_netmask == INADDR_ANY) {
		dev->ip_netmask = inet_class_netmask(dev->ip_addr);
		printf("IP-Config: %s guessed netmask %s\n",
			dev->name, my_inet_ntoa(dev->ip_netmask));
	}
	if (dev->ip_broadcast == INADDR_ANY) {
		dev->ip_broadcast = (dev->ip_addr & dev->ip_netmask) | ~dev->ip_netmask;
		printf("IP-Config: %s guessed broadcast address %s\n",
			dev->name, my_inet_ntoa(dev->ip_broadcast));
	}
	if (dev->ip_nameserver[0] == INADDR_ANY) {
		dev->ip_nameserver[0] = dev->ip_server;
		printf("IP-Config: %s guessed nameserver address %s\n",
			dev->name, my_inet_ntoa(dev->ip_nameserver[0]));
	}
}

static void process_receive_event(struct state *s, time_t now)
{
	switch (s->state) {
	case DEVST_BOOTP:
		s->restart_state = DEVST_BOOTP;
		switch (bootp_recv_reply(s->dev)) {
		case -1:
			s->state = DEVST_ERROR;
			break;
		case 1:
			s->state = DEVST_COMPLETE;
			break;
		}
		break;

	case DEVST_DHCPDISC:
		s->restart_state = DEVST_DHCPDISC;
		switch (dhcp_recv_offer(s->dev)) {
		case -1:
			s->state = DEVST_ERROR;
			break;
		case 1: /* Offer received */
			s->state = DEVST_DHCPREQ;
			dhcp_send_request(s->dev);
			break;
		}
		break;

	case DEVST_DHCPREQ:
		s->restart_state = DEVST_DHCPDISC;
		switch (dhcp_recv_ack(s->dev)) {
		case -1: /* error */
			s->state = DEVST_ERROR;
			break;
		case 1:	/* ACK received */
			s->state = DEVST_COMPLETE;
			break;
		case 2:	/* NAK received */
			s->state = DEVST_DHCPDISC;
			break;
		}
		break;
	}

	switch (s->state) {
	case DEVST_COMPLETE:
		postprocess_device(s->dev);
		configure_device(s->dev);
		dump_device_config(s->dev);
		print_device_config(s->dev);
		break;

	case DEVST_ERROR:
		/* error occurred, try again in 10 seconds */
		s->expire = now + 10;
		break;
	}
}

static void process_timeout_event(struct state *s, time_t now)
{
	int ret = 0;

	/*
	 * Is the link up?  If not, try again in 1 second.
	 */
	if (!netdev_running(s->dev)) {
		s->expire = now + 1;
		s->state = s->restart_state;
		return;
	}

	/*
	 * If we had an error, restore a sane state to
	 * restart from.
	 */
	if (s->state == DEVST_ERROR)
		s->state = s->restart_state;

	/*
	 * Now send a packet depending on our state.
	 */
	switch (s->state) {
	case DEVST_BOOTP:
		ret = bootp_send_request(s->dev);
		s->restart_state = DEVST_BOOTP;
		break;

	case DEVST_DHCPDISC:
		ret = dhcp_send_discover(s->dev);
		s->restart_state = DEVST_DHCPDISC;
		break;

	case DEVST_DHCPREQ:
		ret = dhcp_send_request(s->dev);
		s->restart_state = DEVST_DHCPDISC;
		break;
	}

	if (ret == -1) {
		s->state = DEVST_ERROR;
		s->expire = now + 10;
	} else {
		s->expire = now + s->retry_period;

		s->retry_period *= 2;
		if (s->retry_period > 60)
			s->retry_period = 60;
	}
}

static struct state *slist;

static void do_pkt_recv(int pkt_fd, time_t now)
{
	int ifindex, ret;
	struct state *s;

	ret = packet_peek(&ifindex);
	if (ret < 0)
		return;

	for (s = slist; s; s = s->next)
		if (s->dev->ifindex == ifindex) {
			process_receive_event(s, now);
			break;
		}
}

static int loop(void)
{
#define NR_FDS	1
	struct pollfd fds[NR_FDS];
	struct state *s;
	int pkt_fd;
	int nr = 0;
	time_t now;

	pkt_fd = packet_open();
	if (pkt_fd == -1) {
		perror("packet_open");
		return -1;
	}

	fds[0].fd = pkt_fd;
	fds[0].events = POLLRDNORM;

	now = time(NULL);
	while (1) {
		int timeout = 60;
		int pending = 0;

		for (s = slist; s; s = s->next) {
			if (s->state == DEVST_COMPLETE)
				continue;

			pending++;

			if (s->expire - now <= 0)
				process_timeout_event(s, now);

			if (timeout > s->expire - now)
				timeout = s->expire - now;
		}

		if (pending == 0)
			break;

		if (timeout < 0)
			timeout = 0;

		nr = poll(fds, NR_FDS, timeout * 1000);
		now = time(NULL);

		if (fds[0].revents & POLLRDNORM)
			do_pkt_recv(pkt_fd, now);
	}

	packet_close();

	return 0;
}

static int add_one_dev(struct netdev *dev)
{
	struct state *state;

	state = malloc(sizeof(struct state));
	if (!state)
		return -1;

	state->dev = dev;
	state->expire = time(NULL);
	state->retry_period = 1;

	/*
	 * Select the state that we start from.
	 */
	if (dev->caps & CAP_DHCP && dev->ip_addr == INADDR_ANY) {
		state->restart_state = state->state = DEVST_DHCPDISC;
	} else if (dev->caps & CAP_DHCP) {
		state->restart_state = state->state = DEVST_DHCPREQ;
	} else if (dev->caps & CAP_BOOTP) {
		state->restart_state = state->state = DEVST_BOOTP;
	}

	state->next = slist;
	slist = state;

	return 0;
}

static void add_device(const char *devname)
{
	struct netdev *dev;
	int i;

	dev = malloc(sizeof(struct netdev));
	if (dev == NULL) {
		fprintf(stderr, "%s: out of memory\n", progname);
		exit(1);
	}

	memset(dev, 0, sizeof(struct netdev));

	dev->name = devname;

	if (netdev_init_if(dev) == -1)
		return;

	if (bootp_init_if(dev) == -1)
		return;

	printf("IP-Config: %s hardware address", dev->name);
	for (i = 0; i < dev->hwlen; i++)
		printf("%c%02x", i == 0 ? ' ' : ':', dev->hwaddr[i]);
	printf(" mtu %d%s%s\n", dev->mtu,
		dev->caps & CAP_DHCP  ? " DHCP"  :
		dev->caps & CAP_BOOTP ? " BOOTP" : "",
		dev->caps & CAP_RARP  ? " RARP"  : "");

	if (dev->caps && netdev_up(dev) == 0)
		add_one_dev(dev);
}

int main(int argc, char *argv[])
{
	int c;

	do {
		c = getopt(argc, argv, "td:");
		if (c == EOF)
			break;

		switch (c) {
		case 't':
			break;
		case 'd':
			add_device(optarg);
			break;

		case '?':
			fprintf(stderr, "%s: invalid option -%c\n",
				progname, optopt);
			exit(1);
		}
	} while (1);

	loop();

	return 0;
}
