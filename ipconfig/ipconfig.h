/*
 * ipconfig/ipconfig.h
 */

#define LOCAL_PORT	68
#define REMOTE_PORT	(LOCAL_PORT - 1)

extern __u16 cfg_local_port;
extern __u16 cfg_remote_port;

extern int ipconfig_main(int argc, char *argv[]);

/*
 * Note for gcc 3.2.2:
 *
 * If you're turning on debugging, make sure you get rid of -Os from
 * the gcc command line, or else ipconfig will fail to link.
 */
#if 0
#define IPDBG(x) printf x
#else
#define IPDBG(x) do { } while(0)
#endif
