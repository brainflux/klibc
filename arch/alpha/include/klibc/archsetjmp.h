/*
 * arch/alpha/include/klibc/archsetjmp.h
 */

#ifndef _KLIBC_ARCHSETJMP_H
#define _KLIBC_ARCHSETJMP_H

struct __jmp_buf {
  unsigned long s0;
  unsigned long s1;
  unsigned long s2;
  unsigned long s3;
  unsigned long s4;
  unsigned long s5;
  unsigned long fp;
  unsigned long ra;
  unsigned long gp;
  unsigned long sp;
};

/* Must be an array so it will decay to a pointer when a function is called */
typedef struct __jmp_buf jmp_buf[1];

#endif /* _KLIBC_ARCHSETJMP_H */
