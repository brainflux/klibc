/*
 * sigprocmask.c
 */

#include <signal.h>

int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
  return rt_sigprocmask(how, set, oset, sizeof(sigset_t));
}
