/*
 * lrand48.c
 */

#include <stdlib.h>
#include <stdint.h>

unsigned short __rand48_seed[3];

long mrand48(unsigned short xsubi[3])
{
  uint64_t x;

  /* The xsubi[] array is littleendian by spec */
  x = (uint64_t)xsubi[0] +
    ((uint64_t)xsubi[1] << 16) +
    ((uint64_t)xsubi[2] << 32);

  x = (0x5deece66dULL * x) + 0xb;
  
  xsubi[0] = (unsigned short)x;
  xsubi[1] = (unsigned short)(x >> 16);
  xsubi[2] = (unsigned short)(x >> 32);

  return (long)(uint32_t)(x >> 16);
}

long jrand48(void)
{
  return mrand48(__rand48_seed);
}

long nrand48(unsigned short xsubi[3])
{
  return mrand48(xsubi) & 0x7fffffff;
}

long lrand48(void)
{
  return nrand48(__rand48_seed);
}

