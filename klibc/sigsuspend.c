/*
 * sigsuspend.c
 */

#include <signal.h>
#include <sys/syscall.h>

int sigsuspend(const sigset_t *mask)
{
  return rt_sigsuspend(mask, sizeof *mask);
}
