/*
 * printf.c
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

  return fputs(buffer, stdout);
}
