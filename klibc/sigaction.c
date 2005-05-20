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
  struct sigaction sa = *act;
  if ( !(sa.sa_flags & SA_RESTORER) ) {
    /* The kernel can't be trusted to have a valid default restorer */
    sa.sa_flags |= SA_RESTORER;
    sa.sa_restorer = &__sigreturn;
  }
  
#ifdef __NR_sigaction
  return __sigaction(sig, &sa, oact);
#else
  return __rt_sigaction(sig, &sa, oact, sizeof(sigset_t));
#endif
}
