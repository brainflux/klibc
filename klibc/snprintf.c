/*
 * snprintf.c
 */

#include <stdarg.h>
#include <stddef.h>

int my_vsnprintf(char *buffer, size_t n, const char *format, va_list ap);

int my_snprintf(char *buffer, size_t n, const char *format, ...)
{
  va_list ap;
  int rv;

  va_start(ap, format);
  rv = my_vsnprintf(buffer, n, format, ap);
  va_end(ap);
  return rv;
}
