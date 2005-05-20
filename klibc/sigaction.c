/*
 * sigaction.c
 */

#include <signal.h>
#include <sys/syscall.h>

__extern void __sigreturn(void);
__extern int __sigaction(int, const struct sigaction *, struct sigaction *);
__extern int __rt_sigaction(int, const struct sigaction *, struct sigaction *, size_t);

int sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
#if defined(__i386__) || defined(__x86_64__)
  /* x86-64, and the Fedora i386 kernel, are broken without SA_RESTORER */
  struct sigaction sa = *act;
  act = &sa;

  if ( !(sa.sa_flags & SA_RESTORER) ) {
    /* The kernel can't be trusted to have a valid default restorer */
    sa.sa_flags |= SA_RESTORER;
    sa.sa_restorer = &__sigreturn;
  }
#endif

#ifdef __NR_sigaction
  return __sigaction(sig, act, oact);
#else
  return __rt_sigaction(sig, act, oact, sizeof(sigset_t));
#endif
}
