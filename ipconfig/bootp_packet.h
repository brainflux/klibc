/*
 * ipconfig/bootp_packet.h
 */
#ifndef BOOTP_PACKET_H
#define BOOTP_PACKET_H

#include <sys/uio.h>

struct netdev;

/* packet ops */
#define BOOTP_REQUEST	1
#define BOOTP_REPLY	2

/* your basic bootp packet */
struct bootp_hdr {
	__u8	op;
	__u8	htype;
	__u8	hlen;
	__u8	hops;
	__u32	xid;
	__u16	secs;
	__u16	flags;
	__u32	ciaddr;
	__u32	yiaddr;
	__u32	siaddr;
	__u32	giaddr;
	__u8	chaddr[16];
	char	server_name[64];
	char	boot_file[128];
	/* 312 bytes of extensions */
};

#endif
