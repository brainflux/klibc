/*
 * memcmp.c
 */

#include <memcmp.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
  const unsigned char *c1 = s1, *c2 = s2;
  int d = 0;

  while ( n-- ) {
    d = (int)*c2++ - (int)*c1++;
    if ( d )
      break;
  }

  return d;
}
