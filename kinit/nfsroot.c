#include <arpa/inet.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipconfig.h"
#include "kinit.h"
#include "netdev.h"
#include "nfsmount.h"

static char *sub_client(__u32 client, char *path, size_t len)
{
	struct in_addr addr = { client };
	char buf[len];

	if (strstr(path, "%s") != NULL) {
		if (client == INADDR_NONE) {
			fprintf(stderr, "Root-NFS: no client address\n");
			exit(1);
		}

		snprintf(buf, len, path, inet_ntoa(addr));
		strcpy(path, buf);
	}

	return path;
}

int mount_nfs_root(int argc, char *argv[], int flags)
{
	(void) flags; // FIXME - don't ignore this
	
	struct in_addr addr = { INADDR_NONE };
	__u32 client = INADDR_NONE;
	const int len = 1024;
	struct netdev *dev;
	char *path = NULL;
	char root[len];
	char *x, *opts;
	int ret = 0;
	int a;

	if ((path = get_arg(argc, argv, "nfsroot=")) == NULL) {
		path = (char *) "/tftpboot/%s";
	}

	if (*path == '\0') {
		fprintf(stderr, "Root-NFS: no path\n");
		exit(1);
	}

	a = 1;
	
	if ((opts = strchr(path, ',')) != NULL) {
		*opts++ = '\0';
		argv[a++] = (char *) "-o";
		argv[a++] = opts;
	}

	for (dev = ifaces; dev; dev = dev->next) {
		if (dev->ip_server != INADDR_NONE &&
		    dev->ip_server != INADDR_ANY) {
			addr.s_addr = dev->ip_server;
			client = dev->ip_addr;
			break;
		}
		if (dev->ip_addr != INADDR_NONE &&
		    dev->ip_addr != INADDR_ANY) {
			client = dev->ip_addr;
		}
	}

	if ((x = strchr(path, ':')) == NULL) {
		if (addr.s_addr == INADDR_NONE) {
			fprintf(stderr, "Root-NFS: no server defined\n");
			exit(1);
		}

		snprintf(root, len, "%s:%s", inet_ntoa(addr), path);

		argv[a++] = sub_client(client, root, len);
	} else {
		strcpy(root, path);
		argv[a++] = sub_client(client, root, len);
	}

	DEBUG(("NFS-Root: mounting %s on %s with options '%s'\n",
	       argv[a - 1], local, opts ? opts : "none"));

	argv[a++] = (char *) "/root";
	argv[a] = NULL;

	dump_args(a, argv);

	if ((ret = nfsmount_main(a, argv)) != 0) {
		ret = -1;
		goto done;
	}

done:
	return ret;
}
