#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nfsmount.h"
#include "sunrpc.h"

static int rpc_call_tcp(struct client *clnt, struct rpc *rpc)
{
	int ret;

	rpc->call->hdr.frag_hdr = htonl(LAST_FRAG | rpc->call_len);
	rpc->call->hdr.xid = lrand48();
	rpc->call->hdr.msg_type = __constant_htonl(RPC_CALL);
	rpc->call->rpc_vers = __constant_htonl(2);
	
	if ((ret = write(clnt->sock, rpc->call, rpc->call_len)) == -1) {
		perror("write");
		ret = -errno;
		goto bail;
	}
	else if (ret < rpc->call_len) {
		fprintf(stderr, "short write: %d < %d\n", ret, rpc->call_len);
		ret = -1;
		goto bail;
	}

	if ((ret = read(clnt->sock, rpc->reply, rpc->reply_len)) == -1) {
		perror("read");
		ret = -errno;
		goto bail;
	}
	else if (ret < sizeof(struct rpc_reply)) {
		fprintf(stderr, "short read: %d < %d", ret, rpc->reply_len);
		ret = -1;
		goto bail;
	}
	rpc->reply_len = ret;

	if (!(ntohl(rpc->reply->hdr.frag_hdr) & LAST_FRAG) ||
	    rpc->reply->hdr.xid != rpc->call->hdr.xid ||
	    rpc->reply->hdr.msg_type != __constant_htonl(RPC_REPLY)) {
		fprintf(stderr, "bad reply\n");
		ret = -1;
		goto bail;
	}
	
	if (ntohl(rpc->reply->state) != REPLY_OK) {
		fprintf(stderr, "rpc failed: %d\n", ntohl(rpc->reply->state));
		ret = -(int)ntohl(rpc->reply->state);
		goto bail;
	}
	
	ret = 0;
 bail:
	return ret;
}

struct client *tcp_client(__u32 server, __u16 port, __u32 flags)
{
	struct client *clnt = malloc(sizeof(*clnt));
	struct sockaddr_in addr;
	int sock;

	if (clnt == NULL) {
		perror("malloc");
		goto bail;
	}

	memset(clnt, 0, sizeof(clnt));

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		perror("socket");
		goto bail;
	}

	if ((flags & CLI_RESVPORT) && bindresvport(sock, 0) == -1) {
		perror("bindresvport");
		goto bail;
	}
	
	clnt->sock = sock;
	clnt->call_stub = rpc_call_tcp;
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = server;

	if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("connect");
		goto bail;
	}

	goto done;
 bail:
	if (clnt) {
		free(clnt);
		clnt = NULL;
	}
 done:
	return clnt;
}

struct client *udp_client(__u32 server, __u16 port, __u32 flags)
{
	return NULL;
}

void client_free(struct client *c)
{
	if (c->sock != -1)
		close(c->sock);
	free(c);
}

int rpc_call(struct client *client, struct rpc *rpc)
{
	return client->call_stub(client, rpc);
}
