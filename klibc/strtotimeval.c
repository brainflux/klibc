/*
 * strtotimeval.c
 *
 * Nonstandard function which takes a string and converts it to a
 * struct timeval.  Returns a pointer to the first non-numeric
 * character in the string.
 *
 */

#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

char *strtotimeval(const char *str, struct timeval *tv)
{
  int n;
  char *s;

  tv->tv_sec  = strtoul(str, &s, 10);
  tv->tv_usec = 0;

  if ( *s != '.' )
    return s;

  s++;

  for ( n = 0 ; n < 6 && isdigit(*s) ; n++ )
    tv->tv_usec = tv->tv_usec*10 + (*s++ - '0');

  while ( isdigit(*s) )
    s++;
  
  for ( ; n < 6 ; n++ )
    tv->tv_usec *= 10;

  return s;
}
