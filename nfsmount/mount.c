#include <sys/mount.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/nfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nfsmount.h"
#include "sunrpc.h"

static __u32 mount_port;

struct mount_call_2
{
	struct rpc_call rpc;
	__u32 path_len;
	char path[0];
};


#define NFS_MAXFHSIZE 64

struct nfs_fh
{
	__u16 size;
	char data[NFS_MAXFHSIZE];
} __attribute__((packed));

struct foo
{
	struct rpc_reply reply;
	__u32 status;
};


struct mount_reply_2
{
	struct rpc_reply reply;
	__u32 status;
	struct nfs_fh fh;
} __attribute__((packed));

#define MNT_REPLY_2_MINSIZE (sizeof(struct mount_reply_2) - \
			     sizeof(struct nfs_fh))

static void get_ports(__u32 server, const struct nfs_mount_data *data)
{
	__u32 nfs_ver, mount_ver;
	__u32 proto;
	
	if (data->flags & NFS_MOUNT_VER3) {
		nfs_ver = NFS3_VERSION;
		mount_ver = NFS_MNT3_VERSION;
	} else {
		nfs_ver = NFS2_VERSION;
		mount_ver = NFS_MNT_VERSION;
	}

	proto = (data->flags & NFS_MOUNT_TCP) ? IPPROTO_TCP : IPPROTO_UDP;

	if (nfs_port == 0) {
		nfs_port = portmap(server, NFS_PROGRAM, nfs_ver,
				   proto);
		if (nfs_port == 0) {
			if (proto == IPPROTO_TCP) {
				struct in_addr addr = { server };
				fprintf(stderr, "NFS over TCP not "
					"available from %s\n",
					inet_ntoa(addr));
				exit(1);
			}
			nfs_port = NFS_PORT;
		}
	}

	if (mount_port == 0) {
		mount_port = portmap(server, NFS_MNT_PROGRAM, mount_ver,
				     proto);
		if (mount_port == 0)
			mount_port = MOUNT_PORT;
	}
}

static inline int pad_len(int len)
{
	if (len % 8)
		return (len & 7) + 8;
	return len;
}

static inline void dump_params(__u32 server,
			       const char *path,
			       const struct nfs_mount_data *data)
{
#ifdef NFS_DEBUG
	struct in_addr addr = { server };
	
	printf("NFS params:\n");
	printf("  server = %s, path = \"%s\", ", inet_ntoa(addr), path);
	printf("version = %d, proto = %s\n",
	       data->flags & NFS_MOUNT_VER3 ? 3 : 2,
	       (data->flags & NFS_MOUNT_TCP) ? "tcp" : "udp");
	printf("  mount_port = %d, nfs_port = %d, flags = %08x\n",
	       mount_port, nfs_port, data->flags);
	printf("  rsize = %d, wsize = %d, timeo = %d, retrans = %d\n",
	       data->rsize, data->wsize, data->timeo, data->retrans);
	printf("  acreg (min, max) = (%d, %d), acdir (min, max) = (%d, %d)\n",
	       data->acregmin, data->acregmax, data->acdirmin, data->acdirmax);
	printf("  soft = %d, intr = %d, posix = %d, nocto = %d, noac = %d\n",
	       (data->flags & NFS_MOUNT_SOFT) != 0,
	       (data->flags & NFS_MOUNT_INTR) != 0,
	       (data->flags & NFS_MOUNT_POSIX) != 0,
	       (data->flags & NFS_MOUNT_NOCTO) != 0,
	       (data->flags & NFS_MOUNT_NOAC) != 0);
#endif
}

static inline void dump_fh(const char *data, int len)
{
#ifdef NFS_DEBUG
	int i = 0;
	int max = len - (len % 8);
	
	printf("Root file handle: %d bytes\n", NFS2_FHSIZE);

	while (i < max) {
		int j;
		
		printf("  %4d:  ", i);
		for (j = 0; j < 4; j++) {
			printf("%02x %02x %02x %02x  ",
			       data[i] & 0xff, data[i + 1] & 0xff,
			       data[i + 2] & 0xff, data[i + 3] & 0xff);
		}
		i += j;
		printf("\n");
	}
#endif
}

static inline void dump_fh2(const struct nfs_fh *fh)
{
#ifdef NFS_DEBUG
	dump_fh((const char *) fh, NFS2_FHSIZE);
#endif
}

static inline void dump_fh3(const struct nfs_fh *fh)
{
#ifdef NFS_DEBUG
	dump_fh(fh->data, ntohs(fh->size));
#endif
}

static void nfs_umount_v2(struct nfs_mount_data *data)
{
}

static int nfs_mount_v2(__u32 server, const char *path,
			struct nfs_mount_data *data)
{
	struct mount_call_2 *mnt_call = NULL;
	struct mount_reply_2 mnt_reply;
	struct client *clnt = NULL;
	size_t path_len, call_len;
	char mounted = 0;
	struct rpc rpc;
	int ret = 0;

	get_ports(server, data);
	
	dump_params(server, path, data);
	
	if (data->flags & NFS_MOUNT_TCP) {
		clnt = tcp_client(server, mount_port, CLI_RESVPORT);
	} else {
		clnt = udp_client(server, mount_port, CLI_RESVPORT);
	}

	if (clnt == NULL) {
		goto bail;
	}

	path_len = strlen(path);
	call_len = sizeof(*mnt_call) + pad_len(path_len);
	
	if ((mnt_call = malloc(call_len)) == NULL) {
		perror("malloc");
		goto bail;
	}
	
	memset(mnt_call, 0, sizeof(*mnt_call));

	mnt_call->rpc.program = __constant_htonl(NFS_MNT_PROGRAM);
	mnt_call->rpc.prog_vers = __constant_htonl(NFS_MNT_VERSION);
	mnt_call->rpc.proc = __constant_htonl(MNTPROC_MNT);
	mnt_call->path_len = htonl(path_len);
	memcpy(mnt_call->path, path, path_len);

	rpc.call = (struct rpc_call *) mnt_call;
	rpc.call_len = call_len;
	rpc.reply = (struct rpc_reply *) &mnt_reply;
	rpc.reply_len = sizeof(mnt_reply);

	if (rpc_call(clnt, &rpc) < 0)
		goto bail;

	if (rpc.reply_len < MNT_REPLY_2_MINSIZE) {
		fprintf(stderr, "incomplete reply: %d < %d\n",
			rpc.reply_len, MNT_REPLY_2_MINSIZE);
		goto bail;
	}
	
	if (mnt_reply.status != 0) {
		fprintf(stderr, "mount failed: %d\n",
			ntohl(mnt_reply.status));
		goto bail;
	}
	
	dump_fh2(&mnt_reply.fh);

	data->root.size = NFS_FHSIZE;
	memcpy(data->root.data, &mnt_reply.fh, NFS_FHSIZE);
	memcpy(data->old_root.data, &mnt_reply.fh, NFS_FHSIZE);

	mounted = 1;

	goto done;

 bail:
	if (mounted) {
		nfs_umount_v2(data);
	}
	ret = -1;
	
 done:
	if (mnt_call) {
		free(mnt_call);
	}
	
	if (clnt) {
		client_free(clnt);
	}

	return ret;
}

void nfs_umount_v3(struct nfs_mount_data *data)
{
}

int nfs_mount_v3(__u32 server, const char *path,
		 struct nfs_mount_data *data)
{
	return -1;
}

int nfs_mount(__u32 server, const char *rem_path, const char *path,
	     struct nfs_mount_data *data)
{
	struct in_addr wibble = { server };
	struct sockaddr_in addr;
	char *pathname = NULL;
	char mounted = 0;
	char *hostname;
	int sock = -1;
	int path_len;
	int ret = 0;
	
	hostname = inet_ntoa(wibble);
	path_len = strlen(hostname) + strlen(rem_path) + 2;
	pathname = malloc(path_len);

	if (pathname == NULL) {
		perror("malloc");
		goto bail;
	}
	
	snprintf(pathname, path_len, "%s:%s", hostname, rem_path);
	
	if (data->flags & NFS_MOUNT_VER3) {
		ret = nfs_mount_v3(server, rem_path, data);
	} else {
		ret = nfs_mount_v2(server, rem_path, data);
	}

	if (ret == -1) {
		goto bail;
	}
	mounted = 1;

	if (data->flags & NFS_MOUNT_TCP) {
		sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	} else {
		sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}

	if (sock == -1) {
		perror("socket");
		goto bail;
	}
	
	if (bindresvport(sock, 0) == -1) {
		perror("bindresvport");
		goto bail;
	}
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = server;
	addr.sin_port = htons(nfs_port);
	memcpy(&data->addr, &addr, sizeof(data->addr));

	strncpy(data->hostname, hostname, sizeof(data->hostname));
	
	data->fd = sock;

	ret = mount(pathname, path, "nfs", 0, data);

	if (ret == -1) {
		perror("mount");
		goto bail;
	}
	
	DEBUG(("Mounted %s on %s\n", pathname, path));
	
	goto done;
	
 bail:
	if (mounted) {
		if (data->flags & NFS_MOUNT_VER3) {
			nfs_umount_v3(data);
		} else {
			nfs_umount_v2(data);
		}
	}

	ret = -1;

 done:
	if (pathname) {
		free(pathname);
	}
	
	if (sock != -1) {
		close(sock);
	}
	
	return ret;
}

