/*
 * stdio.h
 */

#ifndef _STDIO_H
#define _STDIO_H

#include <klibc/extern.h>
#include <stdarg.h>
#include <stddef.h>

/* This structure doesn't really exist, but it gives us something
   to define FILE * with */
struct _IO_file;
typedef struct _IO_file FILE;

#define stdin  ((FILE *)0)
#define stdout ((FILE *)1)
#define stderr ((FILE *)2)

#ifndef EOF
# define EOF (-1)
#endif

static __inline__
int fileno(FILE *__f)
{
  /* This should really be intptr_t, but size_t should be the same size */
  return (int)(size_t)__f;
}

__extern int fputs(const char *, FILE *);
__extern int puts(const char *);

__extern size_t __fread(void *, size_t, FILE *);
__extern size_t __fwrite(const void *, size_t, FILE *);

#ifndef __NO_FREAD_FWRITE_INLINES
__extern __inline__ size_t
fread(void *__p, size_t __s, size_t __n, FILE *__f)
{
  return __fread(__p, __s*__n, __f)/__s;
}
__extern __inline__ size_t
fwrite(void *__p, size_t __s, size_t __n, FILE *__f)
{
  return __fwrite(__p, __s*__n, __f)/__s;
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

__extern int sscanf(const char *, const char *, ...);
__extern int vsscanf(const char *, const char *, va_list);

#endif /* _STDIO_H */
