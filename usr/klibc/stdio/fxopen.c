/*
 * fxopen.c
 *
 * Common code between fopen(), fdopen() and the standard descriptors.
 */

#include "stdioint.h"

FILE *stdin, *stdout, *stderr;

/* Doubly-linked list of all stdio structures */
struct _IO_file __stdio_headnode =
{
	.prev = &__stdio_headnode,
	.next = &__stdio_headnode,
};

/* This depends on O_RDONLY == 0, O_WRONLY == 1, O_RDWR == 2 */
int __parse_open_mode(const char *mode)
{
	int plus = 0;
	int flags = O_RDONLY;

	while (*mode) {
		switch (*mode++) {
		case 'r':
			flags = O_RDONLY;
			break;
		case 'w':
			flags = O_WRONLY | O_CREAT | O_TRUNC;
			break;
		case 'a':
			flags = O_WRONLY | O_CREAT | O_APPEND;
			break;
		case '+':
			plus = 1;
			break;
		}
	}

	if (plus)
		flags = (flags & ~(O_RDONLY | O_WRONLY)) | O_RDWR;

	return flags;
}

FILE *__fxopen(int fd, int flags, int close_on_err)
{
	FILE *f = NULL;

	f = malloc(sizeof *f);
	if (!f)
		goto err;

	f->fd = fd;
	f->filepos = lseek(fd, 0, SEEK_CUR);

	f->buf = malloc(BUFSIZ + _IO_UNGET_SLOP);
	if (!f->buf)
		goto err;

	f->bufsiz = BUFSIZ;
	f->offset = _IO_UNGET_SLOP;
	f->bytes = 0;		/* No bytes in buffer */
	f->flags = isatty(fd) ? _IO_FILE_FLAG_LINE_BUF : 0;

	/* Insert into linked list */
	f->prev = &__stdio_headnode;
	f->next = __stdio_headnode.next;
	f->next->prev = f;
	__stdio_headnode.next = f;

	return f;

err:
	if (f) {
		free(f->buf);
		free(f);
	}
	if (close_on_err)
		close(fd);
	errno = ENOMEM;
	return NULL;
}

void __init_stdio(void)
{
	stdin  = __fxopen(0, O_RDONLY, 0);
	stdin->flags = _IO_FILE_FLAG_LINE_BUF;

	stdout = __fxopen(1, O_WRONLY|O_TRUNC, 0);
	stdout->flags = _IO_FILE_FLAG_LINE_BUF;

	stderr = __fxopen(2, O_WRONLY|O_TRUNC, 0);
	stderr->flags = _IO_FILE_FLAG_UNBUF;
}
