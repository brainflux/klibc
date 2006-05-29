/*
 * getopt.c
 *
 * Simple POSIX getopt(), no GNU extensions...
 */

#include <stdint.h>
#include <unistd.h>
#include <string.h>

char *optarg;
int optind, opterr, optopt;
static const char *__optptr;
static const char *__last_optstring;

int getopt(int argc, char *const *argv, const char *optstring)
{
	const char *carg;
	const char *osptr;
	int opt;

	/* getopt() relies on a number of different global
	   state variables, which can make this really
	   confusing if there is more than one use of
	   getopt() in the same program.  This attempts
	   to detect that situation by detecting if
	   the "optstring" argument is the same one as
	   last time we were called; if not, reinitialize
	   the query state. */
	
	if (optstring != __last_optstring ||
	    optind < 1 || optind > argc) {
		/* optind doesn't match the current query */
		__last_optstring = optstring;
		optind = 1;
		__optptr = NULL;
	}

	carg = argv[optind];

	/* First, eliminate all non-option cases */

	if (!carg || carg[0] != '-' || !carg[1]) {
		return -1;
	}

	if (carg[1] == '-' && !carg[2]) {
		optind++;
		return -1;
	}

	if ((uintptr_t) (__optptr - carg) > (uintptr_t) strlen(carg)) {
		/* Someone frobbed optind, change to new opt. */
		__optptr = carg + 1;
	}

	opt = *__optptr++;

	if (opt != ':' && (osptr = strchr(optstring, opt))) {
		if (osptr[1] == ':') {
			if (*__optptr) {
				/* Argument-taking option with attached
				   argument */
				optarg = (char *)__optptr;
				optind++;
			} else {
				/* Argument-taking option with non-attached
				   argument */
				if (argv[optind + 1]) {
					optarg = (char *)argv[optind+1];
					optind += 2;
				} else {
					/* Missing argument */
					optind++;
					return (optstring[0] == ':')
						? ':' : '?';
				}
			}
			return opt;
		} else {
			/* Non-argument-taking option */
			/* __optptr will remember the exact position to
			   resume at */
			if (!*__optptr)
				optind++;
			return opt;
		}
	} else {
		/* Unknown option */
		optopt = opt;
		if (!*__optptr)
			optind++;
		return '?';
	}
}
