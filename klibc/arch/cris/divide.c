#include <stdint.h>
#include <signal.h>

#if BITS == 64
typedef uint64_t uint;
typedef int64_t  sint;
#else
typedef uint32_t uint;
typedef int32_t  sint;
#endif

#ifdef SIGNED
typedef sint xint;
#else
typedef uint xint;
#endif

#ifdef __cris__
static inline uint __attribute__((const)) dstep(uint rs, uint rd) {
  asm("dstep %1,%0" : "+r" (rd) : "r" (rs));
  return rd;
}

static inline uint __attribute__((const)) lz(uint rs) {
  uint rd;
  asm("lz %1,%0" : "=r" (rd) : "r" (rs));
  return rd;
}

#else
/* For testing */
static inline uint __attribute__ ((const)) dstep(uint rs, uint rd) {
  rd <<= 1;
  if ( rd >= rs )
    rd -= rs;

  return rd;
}

static inline uint __attribute__((const)) lz(uint rs) {
  uint rd = 0;
  while ( rs >= 0x7fffffff ) {
    rd++;
    rs <<= 1;
  }
  return rd;
}

#endif

xint NAME (uint num, uint den)
{
  uint quot = 0, qbit = 1;
  int minus = 0;
  xint v;
  
  if ( den == 0 ) {
    raise(SIGFPE);
    return 0;			/* If signal ignored... */
  }

#if SIGNED
  if ( (sint)(num^den) < 0 )
    minus = 1;
  if ( (sint)num < 0 ) num = -num;
  if ( (sint)den < 0 ) den = -den;
#endif

  den--;


  /* Left-justify denominator and count shift */
  while ( (sint)den >= 0 ) {
    den <<= 1;
    qbit <<= 1;
  }

  while ( qbit ) {
    if ( den <= num ) {
      num -= den;
      quot += qbit;
    }
    den >>= 1;
    qbit >>= 1;
  }

  v = (xint)(REM ? num : quot);
  if ( minus ) v = -v;
  return v;
}
