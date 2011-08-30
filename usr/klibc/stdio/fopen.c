/*
 * fopen.c
 */

#include "stdioint.h"

extern int __parse_open_mode(const char *);

FILE *fopen(const char *file, const char *mode)
{
	int flags = __parse_open_mode(mode);
	int fd;

	fd = open(file, flags, 0666);
	if (fd < 0)
		return NULL;

	return __fxopen(fd, flags, 1);
}
