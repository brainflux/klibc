/*
 * arch/arm/syscommon.c
 *
 * Common finishing code for syscalls.  Relies on the fact that
 * r0 is both argument 0 and the return register to avoid writing
 * this in assembly.
 *
 * This routine is branched to from the syscall stubs, so when it
 * returns it returns to the stub-calling routine.
 */

#include <errno.h>

unsigned long __syscall_common(unsigned long rv)
{
  if ( rv < (unsigned long)-4096 ) {
    return rv;
  } else {
    errno = (int) -rv;
    return -1UL;
  }
}
