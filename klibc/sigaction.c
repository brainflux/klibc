/*
 * sigaction.c
 */

#include <signal.h>

int sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
  return rt_sigaction(sig, act, oact, sizeof(sigset_t));
}

