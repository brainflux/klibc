/*
 * sigpending.c
 */

#include <signal.h>

int sigpending(sigset_t *set)
{
  return rt_sigpending(set, sizeof(sigset_t));
}
