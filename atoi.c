/*
 * atoi.c
 */

#include <stdlib.h>
#include "strntoumax.h"

int atoi(const char *nptr)
{
  return (int)strntoumax(nptr, (char **)NULL, 10, ~(size_t)0);
}
