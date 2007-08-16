/*
 * stdio.h
 */

#ifndef _STDIO_H
#define _STDIO_H

#include <klibc/extern.h>
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>

/* Unidirectional buffer */
struct _IO_buf {
	char *buf;		/* Actual buffer */
};

/* Actual FILE structure */
struct _IO_file {
	struct _IO_file *prev, *next;
	off_t filepos;		/* File position */
	char *buf;		/* Buffer */
	int offset;		/* Offset to data in buffer */
	int bytes;		/* Data bytes in buffer */
	int bufsiz;		/* Total size of buffer */
	int fd;			/* Underlying file descriptor */
	int flags;		/* Error, end of file */
};
typedef struct _IO_file FILE;

enum _IO_file_flags {
	_IO_FILE_FLAG_WRITE	=  1, /* Buffer has write data */
	_IO_FILE_FLAG_READ	=  2, /* Buffer has read data */
	_IO_FILE_FLAG_LINE_BUF  =  4, /* Line buffered */
	_IO_FILE_FLAG_UNBUF     =  8, /* Unbuffered */
	_IO_FILE_FLAG_EOF	= 16,
	_IO_FILE_FLAG_ERR	= 32,
};

#ifndef EOF
# define EOF (-1)
#endif

#ifndef BUFSIZ
# define BUFSIZ 65536
#endif

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/*
 * Convert between a FILE * and a file descriptor.
 */
static __inline__ int fileno(FILE * __f)
{
	return __f->fd;
}

__extern FILE *stdin, *stdout, *stderr;

__extern FILE *fopen(const char *, const char *);
__extern FILE *fdopen(int, const char *);
__extern int fclose(FILE *);
__extern int fseek(FILE *, off_t, int);
static __inline__ off_t ftell(FILE * __f)
{
	return __f->filepos;
}

__extern int fputs(const char *, FILE *);
__extern int puts(const char *);
__extern int fputc(int, FILE *);
#define putc(c,f)  fputc((c),(f))
#define putchar(c) fputc((c),stdout)

__extern int fgetc(FILE *);
__extern char *fgets(char *, int, FILE *);
#define getc(f) fgetc(f)
#define getchar() fgetc(stdin)

__extern size_t _fread(void *, size_t, FILE *);
__extern size_t _fwrite(const void *, size_t, FILE *);

#ifndef __NO_FREAD_FWRITE_INLINES
extern __inline__ size_t fread(void *__p, size_t __s, size_t __n, FILE * __f)
{
	return _fread(__p, __s * __n, __f) / __s;
}

extern __inline__ size_t
fwrite(const void *__p, size_t __s, size_t __n, FILE * __f)
{
	return _fwrite(__p, __s * __n, __f) / __s;
}
#endif

__extern int printf(const char *, ...);
__extern int vprintf(const char *, va_list);
__extern int fprintf(FILE *, const char *, ...);
__extern int vfprintf(FILE *, const char *, va_list);
__extern int sprintf(char *, const char *, ...);
__extern int vsprintf(char *, const char *, va_list);
__extern int snprintf(char *, size_t n, const char *, ...);
__extern int vsnprintf(char *, size_t n, const char *, va_list);
__extern int asprintf(char **, const char *, ...);
__extern int vasprintf(char **, const char *, va_list);

__extern int fflush(FILE *);

__extern int sscanf(const char *, const char *, ...);
__extern int vsscanf(const char *, const char *, va_list);

__extern void perror(const char *);

__extern int rename(const char *, const char *);
__extern int renameat(int, const char *, int, const char *);

__extern int remove(const char *);

#endif				/* _STDIO_H */
