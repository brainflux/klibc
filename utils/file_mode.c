#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "file_mode.h"

extern char *progname;

mode_t parse_file_mode(char *arg, mode_t mode, mode_t sumask)
{
	char *clause;

	if (isdigit(*arg) && *arg < '8') {
		unsigned long num;

		num = strtoul(arg, NULL, 8);
		if ((num == ULONG_MAX && errno == ERANGE) || num > 07777) {
			fprintf(stderr, "%s: invalid mode `%s'\n", progname, arg);
			exit(255);
		}
		return (mode_t) num;
	}

	while ((clause = strsep(&arg, ",")) != NULL) {
		mode_t who = 0;
		int action;
		char *p = clause;

		/*
		 * Parse the who list.  Optional.
		 */
		while (1) {
			switch (*p) {
			case 'u':
				who |= S_IRWXU | S_ISUID;
				p++;
				continue;
			case 'g':
				who |= S_IRWXG | S_ISGID;
				p++;
				continue;
			case 'o':
				p++;
				who |= S_IRWXO | S_ISVTX;
				continue;
			case 'a':
				p++;
				who = S_IRWXU|S_IRWXG|S_IRWXO|S_ISUID|S_ISGID|S_ISVTX;
				continue;
			}
			break;
		}

		if (who == 0)
			who = (~sumask) | S_ISVTX;

		/*
		 * Parse an action list.  Must be at least one action.
		 */
		while (*p) {
			mode_t perm = 0;

			/*
			 * Parse the action
			 */
			action = *p;
			if (action == '+' || action == '-' || action == '=')
				p++;

			/*
			 * Parse perm
			 */
			while (*p) {
				switch (*p) {
				case 'r':
					perm |= S_IRUSR|S_IRGRP|S_IROTH;
					*p++;
					continue;
				case 'w':
					perm |= S_IWUSR|S_IWGRP|S_IWOTH;
					*p++;
					continue;
				case 'x':
					perm |= S_IXUSR|S_IXGRP|S_IXOTH;
					*p++;
					continue;
				case 'X':
					perm |= S_ISVTX;
					*p++;
					continue;
				case 's':
					perm |= S_ISUID|S_ISGID;
					*p++;
					continue;
				case 'u':
					perm = mode & S_IRWXU;
					perm |= perm >> 3 | perm >> 6;
					if (mode & S_ISUID)
						perm |= S_ISGID;
					*p++;
					continue;
				case 'g':
					perm = mode & S_IRWXG;
					perm |= perm << 3 | perm >> 3;
					if (mode & S_ISGID)
						perm |= S_ISUID;
					*p++;
					continue;
				case 'o':
					perm = mode & S_IRWXO;
					perm |= perm << 6 | perm << 3;
					*p++;
					continue;
				}
				break;
			}

			perm &= who;

			switch (action) {
			case '+':
				mode |= perm;
				continue;

			case '-':
				mode &= ~perm;
				continue;

			case '=':
				mode &= ~who;
				mode |= perm;
				continue;
			}

			if (!action)
				break;
			fprintf(stderr, "%s: invalid mode `%s'\n", progname, clause);
			exit(255);
		}
	}

	return mode;
}

