#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/klog.h>

static void usage(char *name)
{
	fprintf(stderr, "usage: %s [-c]\n", name);
}

int main(int argc, char *argv[])
{
	char *buf = NULL;
	int bufsz = 0;
	int cmd = 3;	/* Read all messages remaining in the ring buffer */
	int len = 0;
	int opt;
	int i = 0;

	while ((opt = getopt(argc, argv, "c")) != -1) {
		switch (opt) {
		/* Read and clear all messages remaining in the ring buffer */
		case 'c':
			cmd = 4;
			break;
		case '?':
		default:
			usage(argv[0]);
			exit(1);
		}
	}

	if (!bufsz) {
		len = klogctl(10, NULL, 0);	/* Return size of the log buffer */
		if (len > 0)
			bufsz = len;
	}

	if (bufsz) {
		int sz = bufsz + 8;

		buf = (char *)malloc(sz);
		len = klogctl(cmd, buf, sz);
	}

	if (len < 0) {
		perror("klogctl");
		exit(1);
	}

	while (buf[i] && i < len)
		switch (buf[i]) {
		case '<':
			if (i == 0 || buf[i-1] == '\n')
				i++;
		case '0' ... '9':
			if (i > 0 && buf[i-1] == '<')
				i++;
		case '>':
			if (i > 0 && isdigit(buf[i-1]))
				i++;
		default:
			putchar(buf[i++]);
		}

	if (buf[i-1] != '\n')
		putchar('\n');

	return 0;
}
