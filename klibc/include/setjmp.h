/*
 * setjmp.h
 */

#ifndef _SETJMP_H
#define _SETJMP_H

#include <klibc/extern.h>
#include <klibc/compiler.h>
#include <stddef.h>
#include <signal.h>

#include <klibc/archsetjmp.h>

__extern int setjmp(jmp_buf);
__extern __noreturn longjmp(jmp_buf, int);

/*
  Whose bright idea was it to add unrelated functionality to just about
  the only function in the standard C library (setjmp) which cannot be
  wrapped by an ordinary function wrapper?  Anyway, the damage is done,
  and therefore, this wrapper *must* be inline.
*/

struct __sigjmp_buf {
  jmp_buf __jmpbuf;
  sigset_t __sigs;
};

typedef struct __sigjmp_buf sigjmp_buf[1];

__must_inline int sigsetjump(sigjmp_buf __e, int __savesigs)
{
  (void)__savesigs;		/* We always save signals... */
  sigprocmask(0, NULL, &__e->__sigs);
  return setjmp(__e->__jmpbuf);
}

__must_inline __noreturn siglongjmp(sigjmp_buf __e, int __rv)
{
  sigprocmask(SIG_SETMASK, &__e->__sigs, NULL);
  longjmp(__e->__jmpbuf, __rv);
}

#endif /* _SETJMP_H */
