#include <arpa/inet.h>
#include <errno.h>
#include <sys/mount.h>
#include <sys/types.h>
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

static void check_path(const char *path)
{
	struct stat st;

	if (stat(path, &st) == -1) {
		if (errno != ENOENT) {
			perror("stat");
			exit(1);
		}
		if (mkdir(path, 0755) == -1) {
			perror("mkdir");
			exit(1);
		}
	}
	else if (!S_ISDIR(st.st_mode)) {
		fprintf(stderr, "NFS-Root: '%s' not a directory\n", path);
		exit(1);
	}
}

int do_nfsroot(int argc, char *argv[])
{
	struct in_addr addr = { INADDR_NONE };
	__u32 client = INADDR_NONE;
	char *local = "/nfs_root";
	const int len = 1024;
	struct netdev *dev;
	char *kinit = NULL;
	char *path = NULL;
	char do_nfs = 0;
	char root[len];
	char *x, *opts;
	int ret = 0;
	int i, a;

	argv[0] = "NFS-Root";

	DEBUG(("Running nfsroot\n"));

	for (i = a = 1; i < argc; i++) {
		if (strncmp(argv[i], "kinit=", 6) == 0) {
			kinit = argv[i] + 6;
		}
		else if (strcmp(argv[i], "root=/dev/nfs") == 0) {
			do_nfs = 1;
		}
		else if (strncmp(argv[i], "nfsroot=", 8) == 0) {
			path = argv[i] + 8;
		}
	}

	if (!do_nfs) {
		goto done;
	}

	if (path == NULL) {
		path = "/tftpboot/%s";
	}

	if (*path == '\0') {
		fprintf(stderr, "Root-NFS: no path\n");
		exit(1);
	}

	if ((opts = strchr(path, ',')) != NULL) {
		*opts++ = '\0';
		argv[a++] = "-o";
		argv[a++] = opts;
	}

	for (dev = ifaces; dev; dev = dev->next) {
		printf("cli: %08x\n", dev->ip_addr);

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
			fprintf(stderr, "Root-NFS: no server\n");
			exit(1);
		}

		snprintf(root, len, "%s:%s", inet_ntoa(addr), path);

		argv[a++] = sub_client(client, root, len);
	} else {
		strcpy(root, path);
		argv[a++] = sub_client(client, root, len);
	}

	check_path(local);

	DEBUG(("NFS-Root: mounting %s on %s with options '%s'\n",
	       argv[a - 1], local, opts ? opts : "none"));

	argv[a++] = local;
	argv[a] = NULL;

	dump_args(a, argv);

	if ((ret = nfsmount_main(a, argv)) != 0) {
		goto done;
	}

	if (chdir(local) == -1) {
		perror("chdir");
		exit(1);
	}

#ifndef INI_DEBUG
	if (pivot_root(".", "old_root") == -1) {
		perror("pivot_root");
		exit(1);
	}

	if (kinit) {
		char *s = strrchr(kinit, '/');
		if (s) {
			s++;
		}
		execl(kinit, s, NULL);
	}
	execl("/sbin/init", "init", NULL);
	execl("/etc/init", "init", NULL);
	execl("/bin/init", "init", NULL);
	execl("/bin/sh", "sh", NULL);
#else
	printf("NFS-Root: imagine pivot_root and exec\n");
#endif

done:
	return ret;
}
