/*
 * mount.c, by rmk
 */
#include <sys/mount.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *progname;

struct mount_opts {
	const char str[8];
	unsigned long rwmask;
	unsigned long rwset;
	unsigned long rwnoset;
};

struct extra_opts {
	char *str;
	char *end;
	int used_size;
	int alloc_size;
};

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(x[0]))

#define MS_SYNC MS_SYNCHRONOUS

#ifndef MS_MOVE /* please add */
#define MS_MOVE 8192
#endif

#define MS_TYPE	(MS_REMOUNT|MS_BIND|MS_MOVE)

static const struct mount_opts options[] = {
	{ "async",	MS_SYNC,	0,		MS_SYNC		},
	{ "atime",	MS_NOATIME,	0,		MS_NOATIME	},
	{ "bind",	MS_TYPE,	MS_BIND,	0,		},
	{ "dev",	MS_NODEV,	0,		MS_NODEV	},
	{ "diratime",	MS_NODIRATIME,	0,		MS_NODIRATIME	},
	{ "exec",	MS_NOEXEC,	0,		MS_NOEXEC	},
	{ "move",	MS_TYPE,	MS_MOVE,	0		},
	{ "recurse",	MS_REC,		MS_REC,		0		},
	{ "remount",	MS_TYPE,	MS_REMOUNT,	0		},
	{ "ro",		MS_RDONLY,	MS_RDONLY,	0		},
	{ "rw",		MS_RDONLY,	0,		MS_RDONLY	},
	{ "suid",	MS_NOSUID,	0,		MS_NOSUID	},
	{ "sync",	MS_SYNC,	MS_SYNC,	0		},
	{ "verbose",	MS_VERBOSE,	MS_VERBOSE,	0		},
};

static struct extra_opts extra;
static unsigned long rwflag;

static void add_extra_option(char *s)
{
	int len = strlen(s) + 1;			/* +1 for ',' */
	int newlen = extra.used_size + len;

	if (newlen >= extra.alloc_size) {
		char *new;

		new = realloc(extra.str, newlen + 1);	/* +1 for NUL */
		if (!new)
			return;

		extra.end += new - extra.str;
		extra.str = new;
		extra.alloc_size = newlen;
	}

	if (extra.used_size)
		*extra.end = ',';
	strcpy(extra.end, s);
	extra.used_size += len;
}

static unsigned long parse_mount_options(char *arg, unsigned long rwflag)
{
	char *s;

	while ((s = strsep(&arg, ",")) != NULL) {
		char *opt = s;
		unsigned int i;
		int res, no = s[0] == 'n' && s[1] == 'o';

		if (no)
			s += 2;

		for (i = 0, res = 1; i < ARRAY_SIZE(options); i++) {
			res = strcmp(s, options[i].str);

			if (res == 0) {
				rwflag &= ~options[i].rwmask;
				if (no)
					rwflag |= options[i].rwnoset;
				else
					rwflag |= options[i].rwset;
			}
			if (res >= 0)
				break;
		}

		if (res != 0 && s[0])
			add_extra_option(opt);
	}

	return rwflag;
}

static int
do_mount(char *dev, char *dir, char *type, unsigned long rwflag, void *data)
{
	char *s;
	int error = 0;

	while ((s = strsep(&type, ",")) != NULL) {
		if (mount(dev, dir, s, rwflag, data) == -1) {
			error = errno;
			if (error == ENODEV)
				continue;
		}
	}

	if (error) {
		errno = error;
		perror("mount");
		return 255;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	char *type = NULL;
	int c;

	progname = argv[0];
	rwflag = MS_VERBOSE;

	do {
		c = getopt(argc, argv, "o:rt:w");
		if (c == EOF)
			break;
		switch (c) {
		case 'o':
			rwflag = parse_mount_options(optarg, rwflag);
			break;
		case 'r':
			rwflag |= MS_RDONLY;
			break;
		case 't':
			type = optarg;
			break;
		case 'w':
			rwflag &= ~MS_RDONLY;
			break;
		case '?':
			fprintf(stderr, "%s: invalid option -%c\n",
				progname, optopt);
			exit(1);
		}
	} while (1);

	/*
	 * If remount, bind or move was specified, then we don't
	 * have a "type" as such.  Use the dummy "none" type.
	 */
	if (rwflag & MS_TYPE)
		type = "none";

	if (optind + 2 != argc || type == NULL) {
		fprintf(stderr, "Usage: %s [-r] [-w] [-o options] [-t type] "
			"device directory\n", progname);
		exit(1);
	}

	return do_mount(argv[optind], argv[optind + 1], type, rwflag,
		        extra.str);
}
