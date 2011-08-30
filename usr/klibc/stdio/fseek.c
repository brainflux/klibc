/*
 * fseek.c
 */

#include "stdioint.h"

__extern int fseek(FILE *f, off_t where, int whence)
{
	off_t rv;

	fflush(f);

	rv = lseek(f->fd, where, whence);
	if (rv != -1) {
		f->filepos = rv;
		f->bytes = 0;
		f->flags &= ~_IO_FILE_FLAG_READ;
		return 0;
	} else {
		return -1;
	}
}
