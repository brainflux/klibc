/*
 * stdioint.h
 *
 * stdio internals
 */

#ifndef USR_KLIBC_STDIO_STDIOINT_H
#define USR_KLIBC_STDIO_STDIOINT_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

/* Assign this much extra to the input buffer in case of ungetc() */
#define _IO_UNGET_SLOP	32

__extern int __parse_open_mode(const char *mode);
__extern FILE *__fxopen(int fd, int flags, int close_on_err);
__extern int __fputc(int c, FILE *f);

__extern struct _IO_file __stdio_headnode;

#endif /* USR_KLIBC_STDIO_STDIOINT_H */
