/*
 * open.c
 *
 * On 32-bit platforms we need to pass O_LARGEFILE to the open()
 * system call, to indicate that we're 64-bit safe.
 */

#include <unistd.h>
#include <fcntl.h>
#include <bitsize.h>
#include <stdarg.h>

#if _BITSIZE == 32 && !defined(__i386__)

extern int __open(const char *, int, mode_t);

int open(const char *pathname, int flags, ...)
{
	mode_t mode = 0;

	if (flags & O_CREAT) {
		va_list ap;

		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}

	return __open(pathname, flags | O_LARGEFILE, mode);
}

#endif
