/*
 * atol.c
 */

#include <stdlib.h>
#include "strntoumax.h"

long atol(const char *nptr)
{
  return (long)strntoumax(nptr, (char **)NULL, 10, ~(size_t)0);
}
