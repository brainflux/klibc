/*
 * signal.h
 */

#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <extern.h>
#include <sys/types.h>
#include <linux/signal.h>

__extern __sighandler_t signal(int, __sighandler_t);
__extern int sigaction(int, const struct sigaction *, struct sigaction *);
__extern int sigprocmask(int, const sigset_t *, sigset_t *);
__extern int sigpending(sigset_t *);
__extern int sigsuspend(const sigset_t *);
__extern int raise(int);
__extern int kill(pid_t, int);

#endif /* _SIGNAL_H */
