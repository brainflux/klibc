/*
 * abort.c
 */

#include <stdlib.h>
#include <signal.h>

void abort(void)
{
  raise(SIGABRT);
  _exit(255);
}
  
