/*
 * wait.c
 */

#include <sys/wait.h>
#include <sys/types.h>

pid_t wait(int *status)
{
  return waitpid((pid_t)-1, status, 0);
}
