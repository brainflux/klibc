/*
 * ipconfig/packet.c
 *
 * Packet socket handling glue.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if_ether.h>
#include <net/if_packet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#include "netdev.h"
#include "packet.h"

static int pkt_fd;

int packet_open(void)
{
	int fd, one = 1;

	/*
	 * Get a PACKET socket for IP traffic.
	 */
	fd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));
	if (fd == -1) {
		perror("socket");
		return -1;
	}

	/*
	 * We want to broadcast
	 */
	if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &one,
		       sizeof(one)) == -1) {
		perror("SO_BROADCAST");
		close(fd);
		fd = -1;
	}

	pkt_fd = fd;

	return fd;
}

void packet_close(void)
{
	close(pkt_fd);
}

static unsigned int ip_checksum(__u16 *hdr, int len)
{
	unsigned int chksum = 0;

	while (len) {
		chksum += *hdr++;
		chksum += *hdr++;
		len--;
	}
	chksum = (chksum & 0xffff) + (chksum >> 16);
	chksum = (chksum & 0xffff) + (chksum >> 16);
	return (~chksum) & 0xffff;
}

struct header {
	struct iphdr	ip;
	struct udphdr	udp;
} __attribute__((packed));

static struct header ipudp_hdrs = {
	.ip = {
		.ihl		= 5,
		.version	= IPVERSION,
		.frag_off	= __constant_htons(IP_DF),
		.ttl		= 64,
		.protocol	= IPPROTO_UDP,
		.saddr		= INADDR_ANY,
		.daddr		= INADDR_BROADCAST,
	},
	.udp = {
		.source		= __constant_htons(68),
		.dest		= __constant_htons(67),
		.len		= 0,
		.check		= 0,
	},
};

/*
 * Send a packet.  The options are listed in iov[1...iov_len].
 * iov[0] is reserved for the bootp packet header.
 */
int packet_send(struct netdev *dev, struct iovec *iov, int iov_len)
{
	struct sockaddr_ll sll;
	struct msghdr msg = {
		.msg_name	= &sll,
		.msg_namelen	= sizeof(sll),
		.msg_iov	= iov,
		.msg_iovlen	= iov_len,
		.msg_control	= NULL,
		.msg_controllen	= 0,
		.msg_flags	= 0
	};
	int i, len = 0;

	/*
	 * Glue in the ip+udp header iovec
	 */
	iov[0].iov_base = &ipudp_hdrs;
	iov[0].iov_len = sizeof(struct header);

	for (i = 0; i < iov_len; i++)
		len += iov[i].iov_len;

	sll.sll_family   = AF_PACKET;
	sll.sll_protocol = htons(ETH_P_IP);
	sll.sll_ifindex  = dev->ifindex;
	sll.sll_hatype   = dev->hwtype;
	sll.sll_pkttype  = PACKET_BROADCAST;
	sll.sll_halen    = dev->hwlen;
	memcpy(sll.sll_addr, dev->hwbrd, dev->hwlen);

	ipudp_hdrs.ip.tot_len   = htons(len);
	ipudp_hdrs.ip.check     = 0;
	ipudp_hdrs.ip.check     = ip_checksum((__u16 *)&ipudp_hdrs.ip,
						ipudp_hdrs.ip.ihl);

	ipudp_hdrs.udp.len      = htons(len - sizeof(struct iphdr));

	return sendmsg(pkt_fd, &msg, 0);
}

int packet_peek(int *ifindex)
{
	struct sockaddr_ll sll;
	struct iphdr iph;
	int ret, sllen = sizeof(struct sockaddr_ll);

	/*
	 * Peek at the IP header.
	 */
	ret = recvfrom(pkt_fd, &iph, sizeof(struct iphdr),
		       MSG_PEEK, (struct sockaddr *)&sll, &sllen);
	if (ret == -1)
		return -1;

	if (sll.sll_family != AF_PACKET)
		goto discard_pkt;

	if (iph.ihl < 5 || iph.version != IPVERSION)
		goto discard_pkt;

	*ifindex = sll.sll_ifindex;

	return 0;

 discard_pkt:
	recvfrom(pkt_fd, &iph, sizeof(struct iphdr),
		 0, (struct sockaddr *)&sll, &sllen);
	return 0;
}

/*
 * Receive a bootp packet.  The options are listed in iov[1...iov_len].
 * iov[0] must point to the bootp packet header.
 */
int packet_recv(struct iovec *iov, int iov_len)
{
	struct iphdr *ip, iph;
	struct udphdr *udp;
	struct msghdr msg = {
		.msg_name	= NULL,
		.msg_namelen	= 0,
		.msg_iov	= iov,
		.msg_iovlen	= iov_len,
		.msg_control	= NULL,
		.msg_controllen	= 0,
		.msg_flags	= 0
	};
	int ret, iphl;

	ret = recvfrom(pkt_fd, &iph, sizeof(struct iphdr),
		       MSG_PEEK, NULL, NULL);
	if (ret == -1)
		return -1;

	if (iph.ihl < 5 || iph.version != IPVERSION)
		goto discard_pkt;

	iphl = iph.ihl * 4;

	ip = malloc(iphl + sizeof(struct udphdr));
	if (!ip)
		goto discard_pkt;

	udp = (struct udphdr *)((char *)ip + iphl);

	iov[0].iov_base = ip;
	iov[0].iov_len = iphl + sizeof(struct udphdr);

	ret = recvmsg(pkt_fd, &msg, 0);
	if (ret == -1)
		goto free_pkt;

	if (ip_checksum((__u16 *)ip, ip->ihl) != 0)
		goto free_pkt;

	if (ntohs(ip->tot_len) > ret || ip->protocol != IPPROTO_UDP)
		goto free_pkt;

	ret -= 4 * ip->ihl;

	if (udp->source != htons(67) || udp->dest != htons(68))
		goto free_pkt;

	if (ntohs(udp->len) > ret)
		goto free_pkt;

	ret -= sizeof(struct udphdr);

	free(ip);

	return ret;

 free_pkt:
	free(ip);
	return 0;

 discard_pkt:
	recvfrom(pkt_fd, &iph, sizeof(struct iphdr), 0, NULL, NULL);
	return 0;
}
