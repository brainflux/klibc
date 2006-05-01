/*
 * sigaction.c
 */

#include <signal.h>
#include <sys/syscall.h>
#include <klibc/sysconfig.h>

__extern void __sigreturn(void);
__extern int __sigaction(int, const struct sigaction *, struct sigaction *);
__extern int __rt_sigaction(int, const struct sigaction *, struct sigaction *, size_t);

int sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
  int rv;

#if _KLIBC_NEEDS_SA_RESTORER
  struct sigaction sa;

  if ( act && !(act->sa_flags & SA_RESTORER) ) {
    sa = *act;
    act = &sa;

    /* The kernel can't be trusted to have a valid default restorer */
    sa.sa_flags |= SA_RESTORER;
    sa.sa_restorer = &__sigreturn;
  }
#endif

#if _KLIBC_USE_RT_SIG
  rv = __rt_sigaction(sig, act, oact, sizeof(sigset_t));
#else
  rv = __sigaction(sig, act, oact);
#endif

#if _KLIBC_NEEDS_SA_RESTORER
  if ( oact && (oact->sa_restorer == &__sigreturn) ) {
    oact->sa_flags &= ~SA_RESTORER;
  }
#endif

  return rv;
}
