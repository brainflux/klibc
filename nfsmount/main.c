/*
 * nfsmount/main.c
 */

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include <linux/nfs_mount.h>

#include "nfsmount.h"
#include "sunrpc.h"
#include "dummypmap.h"

static char *progname;

static struct nfs_mount_data mount_data = {
	.version = NFS_MOUNT_VERSION,
	.flags = NFS_MOUNT_NONLM | NFS_MOUNT_VER3 | NFS_MOUNT_TCP,
	.rsize = 8192,
	.wsize = 8192,
	.timeo = 7,
	.retrans = 3,
	.acregmin = 3,
	.acregmax = 60,
	.acdirmin = 30,
	.acdirmax = 60,
	.namlen = NAME_MAX,
};

__u32 nfs_port;

static struct int_opts {
	char *name;
	int  *val;
} int_opts[] = {
	{ "port",	&nfs_port },
	{ "rsize",	&mount_data.rsize },
	{ "wsize",	&mount_data.wsize },
	{ "timeo",	&mount_data.timeo },
	{ "retrans",	&mount_data.retrans },
	{ "acregmin",	&mount_data.acregmin },
	{ "acregmax",	&mount_data.acregmax },
	{ "acdirmin",	&mount_data.acdirmin },
	{ "acdirmax",	&mount_data.acdirmax },
	{ NULL,		NULL }
};

static struct bool_opts {
	char *name;
	int  and_mask;
	int  or_mask;
} bool_opts[] = {
	{ "soft",	~NFS_MOUNT_SOFT,	NFS_MOUNT_SOFT },
	{ "hard",	~NFS_MOUNT_SOFT,	0 },
	{ "intr",	~NFS_MOUNT_INTR,	NFS_MOUNT_INTR },
	{ "nointr",	~NFS_MOUNT_INTR,	0 },
	{ "posix",	~NFS_MOUNT_POSIX,	NFS_MOUNT_POSIX },
	{ "noposix",	~NFS_MOUNT_POSIX,	0 },
	{ "cto",	~NFS_MOUNT_NOCTO,	0 },
	{ "nocto",	~NFS_MOUNT_NOCTO,	NFS_MOUNT_NOCTO },
	{ "ac",		~NFS_MOUNT_NOAC,	0 },
	{ "noac",	~NFS_MOUNT_NOAC,	NFS_MOUNT_NOAC },
	{ "lock",	~NFS_MOUNT_NONLM,	0 },
	{ "nolock",	~NFS_MOUNT_NONLM,	NFS_MOUNT_NONLM },
	{ "v2",		~NFS_MOUNT_VER3,	0 },
	{ "v3",		~NFS_MOUNT_VER3,	NFS_MOUNT_VER3 },
	{ "udp",	~NFS_MOUNT_TCP,		0 },
	{ "tcp",	~NFS_MOUNT_TCP,		NFS_MOUNT_TCP },
	{ "broken_suid",~NFS_MOUNT_BROKEN_SUID,	NFS_MOUNT_BROKEN_SUID },
	{ NULL,		0,			0 }
};

static int parse_int(const char *val, const char *ctx)
{
	char *end;
	int ret;

	ret = (int) strtoul(val, &end, 0);
	if (*val == '\0' || *end != '\0') {
		fprintf(stderr, "%s: invalid value for %s\n", val, ctx);
		exit(1);
	}
	return ret;
}

static void parse_opts(char *opts)
{
	char *cp, *val;

	while ((cp = strsep(&opts, ",")) != NULL) {
		if (*cp == '\0')
			continue;
		if ((val = strchr(cp, '=')) != NULL) {
			struct int_opts *opts = int_opts;
			*val++ = '\0';
			while (opts->name && strcmp(opts->name, cp) != 0)
				opts++;
			if (opts->name)
				*(opts->val) = parse_int(val, opts->name);
			else {
				fprintf(stderr, "%s: bad option '%s'\n",
					progname, cp);
				exit(1);
			}
		} else {
			struct bool_opts *opts = bool_opts;
			while (opts->name && strcmp(opts->name, cp) != 0)
				opts++;
			if (opts->name) {
				mount_data.flags &= opts->and_mask;
				mount_data.flags |= opts->or_mask;
			} else {
				fprintf(stderr, "%s: bad option '%s'\n",
					progname, cp);
				exit(1);
			}
		}
	}
}

static __u32 parse_addr(const char *ip)
{
	struct in_addr in;
	if (inet_aton(ip, &in) == 0) {
		fprintf(stderr, "%s: can't parse IP address '%s'\n",
			progname, ip);
		exit(1);
	}
	return in.s_addr;
}

static void check_path(const char *path)
{
	struct stat st;

	if (stat(path, &st) == -1) {
		perror("stat");
		exit(1);
	}
	else if (!S_ISDIR(st.st_mode)) {
		fprintf(stderr, "%s: '%s' not a directory\n",
			progname, path);
		exit(1);
	}
}

int main(int argc, char *argv[])
	__attribute__ ((weak, alias ("nfsmount_main")));

int nfsmount_main(int argc, char *argv[])
{
	struct timeval now;
	__u32 server = 0;
	char *rem_name;
	char *rem_path;
	char *hostname;
	char *path;
	int c;
	int spoof_portmap = 0;
	FILE *portmap_file = NULL;

	progname = argv[0];

	gettimeofday(&now, NULL);
	srand48(now.tv_usec ^ (now.tv_sec << 24));

	while ((c = getopt(argc, argv, "o:p:")) != EOF) {
		switch (c) {
		case 'o':
			parse_opts(optarg);
			break;
		case 'p':
			spoof_portmap = 1;
			portmap_file  = fopen(optarg, "w");
			break;
		case '?':
			fprintf(stderr, "%s: invalid option -%c\n",
				progname, optopt);
			exit(1);
		}
	}

	if (optind == argc) {
		fprintf(stderr, "%s: need a path\n", progname);
		exit(1);
	}

	hostname = rem_path = argv[optind];

	if ((rem_name = strdup(rem_path)) == NULL) {
		perror("strdup");
		exit(1);
	}

	if ((rem_path = strchr(rem_path, ':')) == NULL) {
		fprintf(stderr, "%s: need a server\n", progname);
		exit(1);
	}

	*rem_path++ = '\0';

	if (*rem_path != '/') {
		fprintf(stderr, "%s: need a path\n", progname);
		exit(1);
	}

	server = parse_addr(hostname);

	if (optind <= argc - 2) {
		path = argv[optind + 1];
	} else {
		path = "/nfs_root";
	}

	check_path(path);

	if ( spoof_portmap ) {
		int sock = bind_portmap();

		if ( sock == -1 ) {
			if ( errno == EINVAL || errno == EADDRINUSE )
				spoof_portmap = 0; /* Assume not needed */
			else {
				fprintf(stderr,
					"%s: portmap spoofing failed\n",
					progname);
				exit(1);
			}
		} else {
			spoof_portmap = fork();
			if ( spoof_portmap == -1 ) {
				fprintf(stderr, "%s: cannot fork\n", progname);
				exit(1);
			} else if ( spoof_portmap == 0 ) {
				/* Child process */
				dummy_portmap(sock, portmap_file);
				_exit(255); /* Error */
			} else {
				/* Parent process */
				close(sock);
			}
		}
	}

	if (nfs_mount(rem_name, hostname, server, rem_path, path,
		      &mount_data) != 0)
		return 1;

	/* If we set up the spoofer, tear it down now */
	if ( spoof_portmap ) {
		kill(SIGTERM, spoof_portmap);
		while ( waitpid(spoof_portmap, NULL, 0) == -1 &&
			errno == EINTR );
	}

	free(rem_name);

	return 0;
}
