/*
 * strtotimex.c
 *
 * Nonstandard function which takes a string and converts it to a
 * struct timespec/timeval.  Returns a pointer to the first non-numeric
 * character in the string.
 *
 */

#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

char * NAME (const char *str, TIMEX *ts)
{
  int n;
  char *s, *s0;
  __typeof__(ts->FSEC) fs;	/* Fractional seconds */

  ts->tv_sec = strtoul(str, &s, 10);
  fs = 0;

  if ( *s == '.' ) {
    s0 = s+1;

    fs = strtoul(s0, &s, 10);
    n = s-s0;
    
    while ( isdigit(*s) )
      s++;
    
    for ( ; n < DECIMALS ; n++ )
      fs *= 10;
  }

  ts->FSEC = fs;
  return s;
}
