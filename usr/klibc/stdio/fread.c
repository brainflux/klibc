/*
 * fread.c
 */

#include <string.h>
#include "stdioint.h"

size_t _fread(void *buf, size_t count, FILE *f)
{
	size_t bytes = 0;
	size_t nb;
	char *p = buf;
	ssize_t rv;

	/* Note: one could avoid double-buffering large reads. */

	for (;;) {
		nb = f->bytes;
		nb = (count < nb) ? count : nb;
		if (nb) {
			memcpy(p, f->buf+f->offset, nb);
			f->offset += nb;
			f->bytes  -= nb;
			p += nb;
			count -= nb;
			bytes += nb;
			f->filepos += nb;
			if (!f->bytes)
				f->flags &= ~_IO_FILE_FLAG_READ;
		}

		if (!count)
			break;	/* Done... */

		/* If we get here, f->ibuf must be empty */
		f->offset = _IO_UNGET_SLOP;

		rv = read(f->fd, f->buf+_IO_UNGET_SLOP, BUFSIZ);
		if (rv == -1) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			f->flags |= _IO_FILE_FLAG_ERR;
			return bytes;
		} else if (rv == 0) {
			f->flags |= _IO_FILE_FLAG_EOF;
			return bytes;
		}

		f->bytes = rv;
		f->flags |= _IO_FILE_FLAG_READ;
	}
	return bytes;
}
