/*
 * ipconfig/main.c
 */
#include <errno.h>
#include <poll.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>

#include <linux/if_arp.h>

#include "ipconfig.h"
#include "netdev.h"
#include "bootp_packet.h"
#include "bootp_proto.h"
#include "dhcp_proto.h"
#include "packet.h"

static const char sysfs_class_net[] = "/sys/class/net";
static const char *progname;
static char do_not_config;
static unsigned int default_caps = CAP_DHCP | CAP_BOOTP | CAP_RARP;
static int loop_timeout = -1;
static int configured;

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
	if (do_not_config)
		return;

	if (netdev_setmtu(dev))
		printf("IP-Config: failed to set MTU on %s to %u\n",
		       dev->name, dev->mtu);

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

static void complete_device(struct netdev *dev)
{
	postprocess_device(dev);
	configure_device(dev);
	dump_device_config(dev);
	print_device_config(dev);

	++configured;

	dev->next = ifaces;
	ifaces = dev;
}

static int process_receive_event(struct state *s, time_t now)
{
	int handled = 1;

	switch (s->state) {
	case DEVST_BOOTP:
		s->restart_state = DEVST_BOOTP;
		switch (bootp_recv_reply(s->dev)) {
		case -1:
			s->state = DEVST_ERROR;
			break;
		case 1:
			s->state = DEVST_COMPLETE;
			DEBUG(("\n   bootp reply\n"));
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
		complete_device(s->dev);
		break;

	case DEVST_ERROR:
		/* error occurred, try again in 10 seconds */
		s->expire = now + 10;
	default:
		DEBUG(("\n"));
		handled = 0;
		break;
	}

	return handled;
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
struct netdev *ifaces;

static int do_pkt_recv(int pkt_fd, time_t now)
{
	int ifindex, ret;
	struct state *s;

	ret = packet_peek(&ifindex);
	if (ret < 0)
		goto bail;

	for (s = slist; s; s = s->next) {
		if (s->dev->ifindex == ifindex) {
			ret |= process_receive_event(s, now);
			break;
		}
	}

 bail:
	return ret;
}

static int loop(void)
{
#define NR_FDS	1
	struct pollfd fds[NR_FDS];
	struct state *s;
	int pkt_fd;
	int nr = 0;
	struct timeval now, prev;
	time_t start;

	pkt_fd = packet_open();
	if (pkt_fd == -1) {
		perror("packet_open");
		return -1;
	}

	fds[0].fd = pkt_fd;
	fds[0].events = POLLRDNORM;

	gettimeofday(&now, NULL);
	start = now.tv_sec;
	while (1) {
		int timeout = 60;
		int pending = 0;
		int timeout_ms;
		int x;

		for (s = slist; s; s = s->next) {
			if (s->state == DEVST_COMPLETE)
				continue;

			pending++;

			if (s->expire - now.tv_sec <= 0)
				process_timeout_event(s, now.tv_sec);

			if (timeout > s->expire - now.tv_sec)
				timeout = s->expire - now.tv_sec;
		}

		if (pending == 0)
			break;

		timeout_ms = timeout * 1000;

		for (x = 0; x < 2; x++) {
			int delta_ms;

			if (timeout_ms <= 0)
				timeout_ms = 100;

			nr = poll(fds, NR_FDS, timeout_ms);
			prev = now;
			gettimeofday(&now, NULL);

			if ((fds[0].revents & POLLRDNORM)) {
				nr = do_pkt_recv(pkt_fd, now.tv_sec);
				if (nr == 1)
					break;
				else if (nr == 0)
					packet_discard();
			}

			if (loop_timeout >= 0 &&
			    now.tv_sec - start >= loop_timeout) {
				printf("IP-Config: no response after %d "
				       "secs - giving up\n", loop_timeout);
				goto bail;
			}

			delta_ms = (now.tv_sec - prev.tv_sec) * 1000;
			delta_ms += (now.tv_usec - prev.tv_usec) / 1000;

			DEBUG(("Delta: %d ms\n", delta_ms));

			timeout_ms -= delta_ms;
		}
	}
 bail:
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

static void parse_addr(__u32 *addr, const char *ip)
{
	struct in_addr in;
	if (inet_aton(ip, &in) == 0) {
		fprintf(stderr, "%s: can't parse IP address '%s'\n",
			progname, ip);
		exit(1);
	}
	*addr = in.s_addr;
}

static unsigned int parse_proto(const char *ip)
{
	unsigned int caps = 0;

	if (*ip == '\0' || strcmp(ip, "on") == 0 || strcmp(ip, "any") == 0)
		caps = CAP_BOOTP | CAP_DHCP | CAP_RARP;
	else if (strcmp(ip, "both") == 0)
		caps = CAP_BOOTP | CAP_RARP;
	else if (strcmp(ip, "dhcp") == 0)
		caps = CAP_BOOTP | CAP_DHCP;
	else if (strcmp(ip, "bootp") == 0)
		caps = CAP_BOOTP;
	else if (strcmp(ip, "rarp") == 0)
		caps = CAP_RARP;
	else if (strcmp(ip, "none") == 0 || strcmp(ip, "static") == 0 || strcmp(ip, "off") == 0)
		goto bail;
	else {
		fprintf(stderr, "%s: invalid protocol '%s'\n",
			progname, ip);
		exit(1);
	}
 bail:
	return caps;
}

static void bringup_device(struct netdev *dev)
{
	if (netdev_up(dev) == 0) {
		if (dev->caps) {
			add_one_dev(dev);
		} else {
			complete_device(dev);
		}
	}
}

static void add_all_devices(struct netdev *template);

static int parse_device(struct netdev *dev, const char *ip)
{
	char *cp;
	int i, opt;

	if (strncmp(ip, "ip=", 3) == 0) {
		ip += 3;
	}
	else if (strncmp(ip, "nfsaddrs=", 9) == 0) {
		ip += 9;
	}

	if (strchr(ip, ':') == NULL) {
		dev->name = ip;
		goto done;
	}

	for (i = opt = 0; ip && *ip; ip = cp, opt++) {
		if ((cp = strchr(ip, ':'))) {
			*cp++ = '\0';
		}
		if (opt > 6) {
			fprintf(stderr, "%s: too many options for %s\n",
				progname, dev->name);
			exit(1);
		}

		if (*ip == '\0')
			continue;
		DEBUG(("IP-Config: opt #%d: '%s'\n", opt, ip));
		switch (opt) {
		case 0:
			parse_addr(&dev->ip_addr, ip);
			break;
		case 1:
			parse_addr(&dev->ip_server, ip);
			break;
		case 2:
			parse_addr(&dev->ip_gateway, ip);
			break;
		case 3:
			parse_addr(&dev->ip_netmask, ip);
			break;
		case 4:
			strncpy(dev->hostname, ip, SYS_NMLN - 1);
			dev->hostname[SYS_NMLN - 1] = '\0';
			break;
		case 5:
			dev->name = ip;
			break;
		case 6:
			dev->caps = parse_proto(ip);
			break;
		}
	}
 done:
	if (dev->name == NULL ||
	    dev->name[0] == '\0' ||
	    strcmp(dev->name, "all") == 0) {
		add_all_devices(dev);
		return 0;
	}
	return 1;
}

static void bringup_device(struct netdev *dev)
{
	if (netdev_up(dev) == 0) {
		if (dev->caps) {
			add_one_dev(dev);
		} else {
			complete_device(dev);
		}
	}
}

static void bringup_one_dev(struct netdev *template, struct netdev *dev)
{
		if (template->ip_addr != INADDR_NONE)
			dev->ip_addr = template->ip_addr;
		if (template->ip_server != INADDR_NONE)
			dev->ip_server = template->ip_server;
		if (template->ip_gateway != INADDR_NONE)
			dev->ip_gateway = template->ip_gateway;
		if (template->ip_netmask != INADDR_NONE)
			dev->ip_netmask = template->ip_netmask;
		if (template->ip_nameserver[0] != INADDR_NONE)
			dev->ip_nameserver[0] = template->ip_nameserver[0];
		if (template->ip_nameserver[1] != INADDR_NONE)
			dev->ip_nameserver[1] = template->ip_nameserver[1];
		if (template->hostname[0] != '\0')
			strcpy(dev->hostname, template->hostname);
		dev->caps &= template->caps;

		bringup_device(dev);
}

static struct netdev *add_device(const char *info)
{
	struct netdev *dev;
	int i;

	dev = malloc(sizeof(struct netdev));
	if (dev == NULL) {
		fprintf(stderr, "%s: out of memory\n", progname);
		exit(1);
	}

	memset(dev, 0, sizeof(struct netdev));
	dev->caps = default_caps;

	if (parse_device(dev, info) == 0)
		goto bail;

	if (netdev_init_if(dev) == -1)
		goto bail;

	if (bootp_init_if(dev) == -1)
		goto bail;

	printf("IP-Config: %s hardware address", dev->name);
	for (i = 0; i < dev->hwlen; i++)
		printf("%c%02x", i == 0 ? ' ' : ':', dev->hwaddr[i]);
	printf(" mtu %d%s%s\n", dev->mtu,
		dev->caps & CAP_DHCP  ? " DHCP"  :
		dev->caps & CAP_BOOTP ? " BOOTP" : "",
	       dev->caps & CAP_RARP  ? " RARP"  : "");
	return dev;
 bail:
	free(dev);
	return NULL;
}

static int add_all_devices(struct netdev *template)
{
	DIR *d;
	struct dirent *de;
	struct netdev *dev;
	char t[PATH_MAX], p[255];
	int i, fd;
	unsigned long flags;

	d = opendir(sysfs_class_net);
	if (!d)
		return 0;

	while((de = readdir(d)) != NULL ) {
		/* This excludes devices beginning with dots or "dummy", as well as . or .. */
		if ( de->d_name[0] == '.' || !strcmp(de->d_name, "..") )
			continue;
		i = snprintf(t, PATH_MAX-1, "%s/%s/flags", sysfs_class_net, de->d_name);
		if (i < 0 || i >= PATH_MAX-1)
			continue;
		t[i] = '\0';
		fd = open(t, O_RDONLY);
		if (fd < 0) {
			perror(t);
			continue;
		}
		i = read(fd, &p, sizeof(p) - 1);
		close(fd);
		if (i < 0) {
			perror(t);
			continue;
		}
		p[i] = '\0';
		flags = strtoul(p, NULL, 0);
		/* Heuristic for if this is a reasonable boot interface.  This is the same
		   logic the in-kernel ipconfig uses... */
		if ( !(flags & IFF_LOOPBACK) &&
		     (flags & (IFF_BROADCAST|IFF_POINTOPOINT)) )
			continue;
		if ((dev = add_device(de->d_name)) == NULL)
			continue;
		bringup_one_dev(template, dev);
	}
	closedir(d);
	return 1;
}

static int check_autoconfig(void)
{
	int ndev = 0, nauto = 0;
	struct state *s;

	for (s = slist; s; s = s->next) {
		ndev++;
		if (s->dev->caps)
			nauto++;
	}

	if (ndev == 0) {
		if (configured == 0) {
			fprintf(stderr, "%s: no devices to configure\n",
				progname);
			exit(1);
		}
	}

	return nauto;
}

int main(int argc, char *argv[])
	__attribute__ ((weak, alias ("ipconfig_main")));

int ipconfig_main(int argc, char *argv[])
{
	struct netdev *dev;
	struct timeval now;
	int c, port;

	progname = argv[0];

	gettimeofday(&now, NULL);
	srand48(now.tv_usec ^ (now.tv_sec << 24));

	do {
		c = getopt(argc, argv, "c:d:np:t:");
		if (c == EOF)
			break;

		switch (c) {
		case 'c':
			default_caps = parse_proto(optarg);
			break;
		case 'p':
			port = atoi(optarg);
			if (port <= 0 || port > USHRT_MAX) {
				fprintf(stderr,
					"%s: invalid port number %d\n",
					progname, port);
				exit(1);
			}
			cfg_local_port = port;
			cfg_remote_port = cfg_local_port - 1;
			break;
		case 't':
			loop_timeout = atoi(optarg);
			if (loop_timeout < 0) {
				fprintf(stderr,
					"%s: invalid timeout %d\n",
					progname, loop_timeout);
				exit(1);
			}
			break;
		case 'n':
			do_not_config = 1;
			break;
		case 'd':
			dev = add_device(optarg);
			if (dev)
				bringup_device(dev);
			break;
		case '?':
			fprintf(stderr, "%s: invalid option -%c\n",
				progname, optopt);
			exit(1);
		}
	} while (1);

	for (c = optind; c < argc; c++) {
		dev = add_device(argv[c]);
		if (dev)
			bringup_device(dev);
	}

	if (check_autoconfig()) {
		if (cfg_local_port != LOCAL_PORT) {
			printf("IP-Config: binding source port to %d, "
			       "dest to %d\n",
			       cfg_local_port,
			       cfg_remote_port);
		}
		loop();
	}

	return 0;
}
