#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char *progname;

static __noreturn usage(void)
{
	fprintf(stderr, "Usage: %s name {b|c} major minor\n", progname);
	exit(1);
}

int main(int argc, char *argv[], char *envp[])
{
	char *name = NULL, *endp;
	unsigned int major, minor;
	mode_t mode;
	dev_t dev;

	progname = argv[0];
	if (argc != 5)
		usage();

	name = argv[1];
	if (!name) {
		perror("device");
		usage();
	}

	mode = 0666;
	if (argv[2][0] == 'c')
		mode |= S_IFCHR;
	else if (argv[2][0] == 'b')
		mode |= S_IFBLK;
	else {
		perror("block or char devices.");
		usage();
	}

	major = strtol(argv[3], &endp, 0);
	if (*endp != '\0') {
		perror("major.");
		usage();
	}
	minor = strtol(argv[4], &endp, 0);
	if (*endp != '\0') {
		perror("minor.");
		usage();
	}
	dev = makedev(major, minor);

	if (mknod(name, mode, dev) == -1) {
		perror("mknod");
		exit(1);
	}

	exit(0);
}
