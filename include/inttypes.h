/*
 * inttypes.h
 */

#ifndef _INTTYPES_H
#define _INTTYPES_H

#include <extern.h>
#include <stdint.h>

static __inline__ intmax_t imaxabs(intmax_t __n)
{
  return (__n < (intmax_t)0) ? -__n : __n;
}

__extern intmax_t strtoimax(const char *, char **, int);
__extern uintmax_t strtoumax(const char *, char **, int);

__extern uintmax_t strntoumax(const char *, char **, int, size_t);

#endif /* _INTTYPES_H */
