/*
 * signal.c
 */

#include <signal.h>
#include <string.h>

__sighandler_t signal(int signum, __sighandler_t handler)
{
  struct sigaction sa;

  sa.sa_handler = handler;
  sa.sa_flags   = SA_RESETHAND;	/* SysV/Linux signal() semantic */
  sa.sa_mask    = 0;		/* Block no other signals */

  if ( sigaction(signum, &sa, &sa) ) {
    return (__sighandler_t)SIG_ERR;
  } else {
    return (__sighandler_t)sa.sa_handler;
  }
}

       
