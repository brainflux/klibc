/*
 * ipconfig/ipconfig.h
 */

#define LOCAL_PORT	68
#define REMOTE_PORT	(LOCAL_PORT - 1)

extern __u16 local_port;
extern __u16 remote_port;

/*
 * Note for gcc 3.2.2:
 *
 * If you're turning on debugging, make sure you get rid of -Os from
 * the gcc command line, or else ipconfig will fail to link.
 */
#if 0
#define DEBUG(x) printf x
#else
#define DEBUG(x) do { } while(0)
#endif
