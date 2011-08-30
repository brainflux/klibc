/*
 * ungetc.c
 */

#include "stdioint.h"

int ungetc(int c, FILE *f)
{
	if (f->flags & _IO_FILE_FLAG_WRITE || f->offset <= 0)
		return EOF;

	f->buf[--f->offset] = c;
	f->bytes++;
	f->flags |= _IO_FILE_FLAG_READ;
	return c;
}
