/*
 * arch/i386/include/klibc/archsetjmp.h
 */

#ifndef _KLIBC_ARCHSETJMP_H
#define _KLIBC_ARCHSETJMP_H

typedef struct __jmp_buf {
  unsigned int ebx;
  unsigned int esp;
  unsigned int ebp;
  unsigned int esi;
  unsigned int edi;
  unsigned int eip;
};

typedef struct __jmp_buf jmp_buf[1];

#endif /* _SETJMP_H */
