/*
 * sunrpc.h - open-coded SunRPC structures
 */

#define SUNRPC_PORT	111
#define MOUNT_PORT	627

#define RPC_CALL	0
#define RPC_REPLY	1

#define PORTMAP_PROGRAM	100000

#define RPC_PMAP_PROGRAM	100000
#define RPC_PMAP_VERSION	2
#define RPC_PMAP_PORT		111

#define PMAP_PROC_GETPORT	3

#define LAST_FRAG	0x80000000

#define REPLY_OK	0

struct rpc_udp_header
{
	__u32 xid;
	__u32 msg_type;
};

struct rpc_header
{
	__u32 frag_hdr;
	struct rpc_udp_header udp;
};

struct rpc_call
{
	struct rpc_header hdr;
	__u32 rpc_vers;

	__u32 program;
	__u32 prog_vers;
	__u32 proc;
	__u32 cred_flavor;

	__u32 cred_len;
	__u32 vrf_flavor;
	__u32 vrf_len;
};

struct rpc_reply 
{
	struct rpc_header hdr;
	__u32 reply_state;
	__u32 vrf_flavor;
	__u32 vrf_len;
	__u32 state;
};

struct rpc
{
	struct rpc_call *call;
	size_t call_len;
	struct rpc_reply *reply;
	size_t reply_len;
};

struct client;

typedef int (*call_stub)(struct client *, struct rpc *);

struct client
{
	int sock;
	call_stub call_stub;
};

#define CLI_RESVPORT	00000001

struct client *tcp_client(__u32 server, __u16 port, __u32 flags);
struct client *udp_client(__u32 server, __u16 port, __u32 flags);
void client_free(struct client *client);

int rpc_call(struct client *client, struct rpc *rpc);

__u32 portmap(__u32 server, __u32 program, __u32 version, __u32 proto);
