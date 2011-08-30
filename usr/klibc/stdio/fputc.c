/*
 * fputc.c
 */

#include "stdioint.h"

int __fputc(int c, FILE *f)
{
	if (f->bytes >= f->bufsiz)
		fflush(f);

	*f->buf++ = c;
	f->flags |= _IO_FILE_FLAG_WRITE;

	if (f->flags & _IO_FILE_LINE_BUF && c == '\n')
		fflush(f);
}

int fputc(int c, FILE *f)
{
	__fputc(c, f);
	if (f->flags & _IO_FILE_UNBUF)
		fflush(f);
}
