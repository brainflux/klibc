/*
 * arch/arm/syscommon.c
 *
 * Common code for syscalls.  Relies on the fact that ARM doesn't insert
 * "padding registers" to deal with 64-bit values.
 */

#include <errno.h>
#include <asm/unistd.h>		/* Can't include sys/syscall.h */

#undef __syscall		/* Macro in asm/unistd.h */

#ifndef __thumb__

/* ARM version */

unsigned long __syscall(unsigned long nr,
			unsigned long a0,
			unsigned long a1,
			unsigned long a2,
			unsigned long a3,
			unsigned long a4,
			unsigned long a5)
{
  register unsigned long rnr asm("r0") = nr;
  register unsigned long ra0 asm("r1") = a0;
  register unsigned long ra1 asm("r2") = a1;
  register unsigned long ra2 asm("r3") = a2;
  register unsigned long ra3 asm("r4") = a3;
  register unsigned long ra4 asm("r5") = a4;
  register unsigned long ra5 asm("r6") = a5;
  register unsigned long rv  asm("r0");

  asm volatile("swi %8"
	       : "=r" (rv)
	       : "r" (rnr), "r" (ra0), "r" (ra1), "r" (ra2),
	       "r" (ra3), "r" (ra4), "r" (ra5),
	       "i" (__NR_syscall));
  
  if ( rv < (unsigned long)-4096 ) {
    return rv;
  } else {
    errno = (int) -rv;
    return -1UL;
  }
}

#else

/* Thumb version */

unsigned long __syscall(unsigned long nr,
			unsigned long a0,
			unsigned long a1,
			unsigned long a2,
			unsigned long a3,
			unsigned long a4,
			unsigned long a5)
{
  register unsigned long rnr asm("r7") = nr;
  register unsigned long ra0 asm("r0") = a0;
  register unsigned long ra1 asm("r1") = a1;
  register unsigned long ra2 asm("r2") = a2;
  register unsigned long ra3 asm("r3") = a3;
  register unsigned long ra4 asm("r4") = a4;
  register unsigned long ra5 asm("r5") = a5;
  register unsigned long rv  asm("r0");

  asm volatile("swi 0"
	       : "=r" (rv)
	       : "r" (rnr), "r" (ra0), "r" (ra1), "r" (ra2),
	       "r" (ra3), "r" (ra4), "r" (ra5));
  
  if ( rv < (unsigned long)-4096 ) {
    return rv;
  } else {
    errno = (int) -rv;
    return -1UL;
  }
}

#endif

