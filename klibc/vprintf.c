/*
 * vprintf.c
 */

#include <stdio.h>
#include <unistd.h>
#include <xio.h>

#define BUFFER_SIZE	16384

int vprintf(const char *format, va_list ap)
{
  int rv;
  char buffer[BUFFER_SIZE];

  rv = vsnprintf(buffer, sizeof buffer, format, ap);

  if ( rv < 0 )
    return rv;

  return __xwrite(1, buffer, rv);
}
