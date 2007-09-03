/*
 * Enough portmapper functionality that mount doesn't hang trying
 * to start lockd.  Enables nfsroot with locking functionality.
 *
 * Note: the kernel will only speak to the local portmapper
 * using RPC over UDP.
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

#include "dummypmap.h"
#include "sunrpc.h"

extern const char *progname;

struct portmap_call {
	struct rpc_call rpc;
	uint32_t program;
	uint32_t version;
	uint32_t proto;
	uint32_t port;
};

struct portmap_reply {
	struct rpc_reply rpc;
	uint32_t port;
};

static int bind_portmap(void)
{
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in sin;

	if (sock < 0)
		return -1;

	memset(&sin, 0, sizeof sin);
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(0x7f000001);	/* 127.0.0.1 */
	sin.sin_port = htons(RPC_PMAP_PORT);
	if (bind(sock, (struct sockaddr *)&sin, sizeof sin) < 0) {
		int err = errno;
		close(sock);
		errno = err;
		return -1;
	}

	return sock;
}

static const char *protoname(uint32_t proto)
{
	switch (ntohl(proto)) {
	case IPPROTO_TCP:
		return "tcp";
	case IPPROTO_UDP:
		return "udp";
	default:
		return NULL;
	}
}

static int dummy_portmap(int sock, FILE *portmap_file)
{
	struct sockaddr_in sin;
	int pktlen, addrlen;
	union {
		struct portmap_call c;
		unsigned char b[65536];	/* Max UDP packet size */
	} pkt;
	struct portmap_reply rply;

	for (;;) {
		addrlen = sizeof sin;
		pktlen = recvfrom(sock, &pkt.c.rpc.hdr.udp, sizeof pkt, 0,
				  (struct sockaddr *)&sin, &addrlen);

		if (pktlen < 0) {
			if (errno == EINTR)
				continue;

			return -1;
		}

		/* +4 to skip the TCP fragment header */
		if (pktlen + 4 < sizeof(struct portmap_call))
			continue;	/* Bad packet */

		if (pkt.c.rpc.hdr.udp.msg_type != htonl(RPC_CALL))
			continue;	/* Bad packet */

		memset(&rply, 0, sizeof rply);

		rply.rpc.hdr.udp.xid = pkt.c.rpc.hdr.udp.xid;
		rply.rpc.hdr.udp.msg_type = htonl(RPC_REPLY);

		if (pkt.c.rpc.rpc_vers != htonl(2)) {
			rply.rpc.reply_state = htonl(REPLY_DENIED);
			/* state <- RPC_MISMATCH == 0 */
		} else if (pkt.c.rpc.program != htonl(PORTMAP_PROGRAM)) {
			rply.rpc.reply_state = htonl(PROG_UNAVAIL);
		} else if (pkt.c.rpc.prog_vers != htonl(2)) {
			rply.rpc.reply_state = htonl(PROG_MISMATCH);
		} else if (pkt.c.rpc.cred_len != 0 || pkt.c.rpc.vrf_len != 0) {
			/* Can't deal with credentials data; the kernel
			   won't send them */
			rply.rpc.reply_state = htonl(SYSTEM_ERR);
		} else {
			switch (ntohl(pkt.c.rpc.proc)) {
			case PMAP_PROC_NULL:
				break;
			case PMAP_PROC_SET:
				if (pkt.c.proto == htonl(IPPROTO_TCP) ||
				    pkt.c.proto == htonl(IPPROTO_UDP)) {
					if (portmap_file)
						fprintf(portmap_file,
							"%u %u %s %u\n",
							ntohl(pkt.c.program),
							ntohl(pkt.c.version),
							protoname(pkt.c.proto),
							ntohl(pkt.c.port));
					rply.port = htonl(1);	/* TRUE = success */
				}
				break;
			case PMAP_PROC_UNSET:
				rply.port = htonl(1);	/* TRUE = success */
				break;
			case PMAP_PROC_GETPORT:
				break;
			case PMAP_PROC_DUMP:
				break;
			default:
				rply.rpc.reply_state = htonl(PROC_UNAVAIL);
				break;
			}
		}

		sendto(sock, &rply.rpc.hdr.udp, sizeof rply - 4, 0,
		       (struct sockaddr *)&sin, addrlen);
	}
}

pid_t start_dummy_portmap(const char *file)
{
	FILE *portmap_filep;
	int sock;
	pid_t spoof_portmap;

	portmap_filep = fopen(file, "w");
	if (!portmap_filep) {
		fprintf(stderr, "%s: cannot write portmap file: %s\n",
			progname, file);
		return -1;
	}

	sock = bind_portmap();
	if (sock == -1) {
		if (errno == EINVAL || errno == EADDRINUSE)
			return 0;	/* Assume not needed */
		else {
			fprintf(stderr, "%s: portmap spoofing failed\n",
				progname);
			return -1;
		}
	}

	spoof_portmap = fork();
	if (spoof_portmap == -1) {
		fprintf(stderr, "%s: cannot fork\n", progname);
		return -1;
	} else if (spoof_portmap == 0) {
		/* Child process */
		dummy_portmap(sock, portmap_filep);
		_exit(255);	/* Error */
	} else {
		/* Parent process */
		close(sock);
		return spoof_portmap;
	}
}
