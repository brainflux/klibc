/*
 * fdopen.c
 */

#include "stdioint.h"

FILE *fdopen(int fd, const char *mode)
{
	int flags = __parse_open_mode(mode);
	int oldflags;

	if (fcntl(fd, F_GETFL, &oldflags))
		return NULL;

	oldflags = (oldflags & ~O_APPEND) | (flags & O_APPEND);
	if (fcntl(fd, F_SETFL, &oldflags))
		return NULL;

	return __fxopen(fd, flags, 0);
}
