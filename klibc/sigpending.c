/*
 * sigpending.c
 */

#include <signal.h>
#include <sys/syscall.h>

int sigpending(sigset_t *set)
{
  return rt_sigpending(set, sizeof(sigset_t));
}
