#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char *progname;

static __noreturn usage(void)
{
	fprintf(stderr, "Usage: %s name {b|c|f} major minor\n", progname);
	exit(1);
}

int main(int argc, char *argv[])
{
	char *name, *type, typec, *endp;
	unsigned int major, minor;
	mode_t mode;
	dev_t dev;

	progname = *argv++;

	name = *argv++;
	if (!name)
		usage();

	type = *argv++;
	if (!type || !type[0] || type[1])
		usage();
	typec = type[0];

	mode = 0;
	switch (type[0]) {
	case 'c':
		mode = S_IFCHR;
		break;
	case 'b':
		mode = S_IFBLK;
		break;
	case 'f':
		mode = S_IFIFO;
		break;
	default:
		usage();
	}

	if (mode == S_IFIFO) {
		dev = 0;
	} else {
		if (!argv[0] || !argv[1])
			usage();
			
		major = strtol(*argv++, &endp, 0);
		if (*endp != '\0')
			usage();
		minor = strtol(*argv++, &endp, 0);
		if (*endp != '\0')
			usage();
		dev = makedev(major, minor);
	}

	if (*argv)
		usage();

	if (mknod(name, mode|0666, dev) == -1) {
		perror("mknod");
		exit(1);
	}

	exit(0);
}
