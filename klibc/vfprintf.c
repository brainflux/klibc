/*
 * vfprintf.c
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <xio.h>

#define BUFFER_SIZE	32768

int vfprintf(FILE *file, const char *format, va_list ap)
{
  int rv;
  char buffer[BUFFER_SIZE];

  rv = vsnprintf(buffer, BUFFER_SIZE, format, ap);

  if ( rv < 0 )
    return rv;

  if ( rv > BUFFER_SIZE-1 )
    rv = BUFFER_SIZE-1;

  return __xwrite((int)file, buffer, rv);
}
