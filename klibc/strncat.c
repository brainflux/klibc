/*
 * strncat.c
 */

#include <string.h>
#include <klibc/compiler.h>

char *strncat(char *dst, const char *src, size_t n)
{
  char *q = strchr(dst, '\0');
  size_t nn = q-dst;

  if ( __likely(nn <= n) )
    n = nn;

  memcpy(q, src, n);
  q[n] = '\0';

  return dst;
}
