/*
 * fflush.c
 */

#include "stdioint.h"

int fflush(FILE *f)
{
	ssize_t rv;
	char *p;

	if (!f) {
		int err = 0;

		for (f = __stdio_headnode.next; f != &__stdio_headnode;
		     f = f->next)
			err |= fflush(f);
		return err;
	}

	if (!(f->flags & _IO_FILE_FLAG_WRITE))
		return 0;

	p = f->buf;
	while (f->bytes) {
		rv = write(f->fd, p, f->bytes);
		if (rv == -1) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			f->flags |= _IO_FILE_FLAG_ERR;
			return EOF;
		} else if (rv == 0) {
			/* EOF on output? */
			f->flags |= _IO_FILE_FLAG_EOF;
			return EOF;
		}

		p += rv;
		f->bytes -= rv;
	}
	f->offset = _IO_UNGET_SLOP;
	f->flags &= ~_IO_FILE_FLAG_WRITE;

	return 0;
}
