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
  int us;

  tv->tv_sec = strtoul(str, &s, 10);
  us = 0;

  if ( *s == '.' ) {
    s++;

    for ( n = 0 ; n < 9 && isdigit(*s) ; n++ )
      us = us*10 + (*s++ - '0');
    
    while ( isdigit(*s) )
      s++;
    
    for ( ; n < 9 ; n++ )
      us *= 10;
  }

  tv->tv_usec = us;
  return s;
}
