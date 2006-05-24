#include <string.h>
#include "kinit.h"

/*
 * Routines that hunt for a specific argument.  Please note that
 * they actually search the array backwards.  That is because on the
 * kernel command lines, it's legal to override an earlier argument
 * with a later argument.
 */

/* Was this boolean argument passed? */
int get_flag(int argc, char *argv[], const char *name)
{
	int i;

	for (i = argc-1; i > 0; i--) {
		if (!strcmp(argv[i], name))
			return 1;
	}
	return 0;
}

/* Was this parameter passed? */
char *get_arg(int argc, char *argv[], const char *name)
{
	int len = strlen(name);
	char *ret = NULL;
	int i;

	for (i = argc-1; i > 0; i--) {
		if (argv[i] && strncmp(argv[i], name, len) == 0 &&
		    (argv[i][len] != '\0')) {
			ret = argv[i] + len;
			break;
		}
	}

	return ret;
}
