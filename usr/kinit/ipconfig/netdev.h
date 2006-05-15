/*
 * ipconfig/netdev.h
 */
#include <sys/utsname.h>
#include <net/if.h>

#define BPLEN		40

struct netdev {
	const char *name;	/* Device name          */
	unsigned int ifindex;	/* interface index      */
	unsigned int hwtype;	/* ARPHRD_xxx           */
	unsigned int hwlen;	/* HW address length    */
	__u8 hwaddr[16];	/* HW address           */
	__u8 hwbrd[16];		/* Broadcast HW address */
	unsigned int mtu;	/* Device mtu           */
	unsigned int caps;	/* Capabilities         */
	time_t open_time;

	struct {		/* BOOTP/DHCP info      */
		int fd;
		__u32 xid;
		__u32 gateway;	/* BOOTP/DHCP gateway   */
	} bootp;

	struct {		/* RARP information     */
		int fd;
	} rarp;

	__u32 ip_addr;		/* my address           */
	__u32 ip_broadcast;	/* broadcast address    */
	__u32 ip_server;	/* server address       */
	__u32 ip_netmask;	/* my subnet mask       */
	__u32 ip_gateway;	/* my gateway           */
	__u32 ip_nameserver[2];	/* two nameservers      */
	__u32 serverid;		/* dhcp serverid        */
	char hostname[SYS_NMLN];	/* hostname             */
	char dnsdomainname[SYS_NMLN];	/* dns domain name      */
	char nisdomainname[SYS_NMLN];	/* nis domain name      */
	char bootpath[BPLEN];	/* boot path            */
	struct netdev *next;	/* next configured i/f  */
};

/*
 * Device capabilities
 */
#define CAP_BOOTP	(1<<0)
#define CAP_DHCP	(1<<1)
#define CAP_RARP	(1<<2)

/*
 * Device states
 */
#define DEVST_UP	0
#define DEVST_BOOTP	1
#define DEVST_DHCPDISC	2
#define DEVST_DHCPREQ	3
#define DEVST_COMPLETE	4
#define DEVST_ERROR	5

int netdev_getflags(struct netdev *dev, short *flags);
int netdev_setaddress(struct netdev *dev);
int netdev_setdefaultroute(struct netdev *dev);
int netdev_up(struct netdev *dev);
int netdev_down(struct netdev *dev);
int netdev_init_if(struct netdev *dev);
int netdev_setmtu(struct netdev *dev);

static inline int netdev_running(struct netdev *dev)
{
	short flags;
	int ret = netdev_getflags(dev, &flags);

	return ret ? 0 : !!(flags & IFF_RUNNING);
}
