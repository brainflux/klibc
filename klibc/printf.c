/*
 * printf.c
 */

#include <stdio.h>
#include <unistd.h>
#include <xio.h>

#define BUFFER_SIZE	16384

int printf(const char *format, ...)
{
  va_list ap;
  int rv;
  char buffer[BUFFER_SIZE];

  va_start(ap, format);
  rv = vsnprintf(buffer, sizeof buffer, format, ap);
  va_end(ap);

  if ( rv < 0 )
    return rv;

  return __xwrite(1, buffer, rv);
}
