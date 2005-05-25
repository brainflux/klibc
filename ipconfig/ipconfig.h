/*
 * ipconfig/ipconfig.h
 */

#include <sys/types.h>
#include <linux/types.h>	/* for __u16 */

#define LOCAL_PORT	68
#define REMOTE_PORT	(LOCAL_PORT - 1)

extern __u16 cfg_local_port;
extern __u16 cfg_remote_port;

extern struct netdev *ifaces;

extern int ipconfig_main(int argc, char *argv[]);

/*
 * Note for gcc 3.2.2:
 *
 * If you're turning on debugging, make sure you get rid of -Os from
 * the gcc command line, or else ipconfig will fail to link.
 */
#undef IPC_DEBUG

#undef DEBUG
#ifdef IPC_DEBUG
#define DEBUG(x) printf x
#else
#define DEBUG(x) do { } while(0)
#endif
