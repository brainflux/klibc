/*
 * resume.c
 *
 * Handle resume from suspend-to-disk
 */

#include <stdio.h>
#include <stdlib.h>

#include "resume.h"

char *progname;

static __noreturn usage(void)
{
	fprintf(stderr, "Usage: %s /dev/<resumedevice>\n", progname);
	exit(1);
}

int main(int argc, char *argv[], char *envp[])
{
	progname = argv[0];
	if (argc != 2)
		usage();

	return resume(argv[1]);
}
