/*
 * nfsmount/nfsmount.h
 */

#include <linux/nfs_mount.h>

extern __u32 nfs_port;

extern int nfsmount_main(int argc, char *argv[]);
int nfs_mount(const char *rem_name, const char *hostname,
	      __u32 server, const char *rem_path,
	      const char *path, struct nfs_mount_data *data);

enum nfs_proto {
	v2 = 2,
	v3,
};

/*
 * Note for gcc 3.2.2:
 *
 * If you're turning on debugging, make sure you get rid of -Os from
 * the gcc command line, or else ipconfig will fail to link.
 */
#undef NFS_DEBUG

#undef DEBUG
#ifdef NFS_DEBUG
#define DEBUG(x) printf x
#else
#define DEBUG(x) do { } while(0)
#endif
