/*
 * signal.c
 */

#include <signal.h>

#if __x86_64__
/* Quick hack: Add sigaction to this file. Not a good solution, it should 
   be in signal.h. 
*/
struct sigaction {
	__sighandler_t sa_handler;
	unsigned long sa_flags;
	void (*sa_restorer)(void);
	sigset_t sa_mask;		/* mask last for extensibility */
};
#endif

__sighandler_t signal(int signum, __sighandler_t handler)
{
  struct sigaction sa;

  sa.sa_handler = handler;
  sa.sa_flags   = SA_RESETHAND;	/* SysV/Linux signal() semantic */
  sigemptyset(&sa.sa_mask);

  if ( sigaction(signum, &sa, &sa) ) {
    return (__sighandler_t)SIG_ERR;
  } else {
    return (__sighandler_t)sa.sa_handler;
  }
}

       
