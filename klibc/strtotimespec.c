/*
 * strtotimespec.c
 *
 * Nonstandard function which takes a string and converts it to a
 * struct timespec.  Returns a pointer to the first non-numeric
 * character in the string.
 *
 */

#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

char *strtotimespec(const char *str, struct timespec *ts)
{
  int n;
  char *s;

  ts->tv_sec  = strtoul(str, &s, 10);
  ts->tv_nsec = 0;

  if ( *s != '.' )
    return s;

  s++;

  for ( n = 0 ; n < 9 && isdigit(*s) ; n++ )
    ts->tv_nsec = ts->tv_nsec*10 + (*s++ - '0');

  while ( isdigit(*s) )
    s++;
  
  for ( ; n < 9 ; n++ )
    ts->tv_nsec *= 10;

  return s;
}
