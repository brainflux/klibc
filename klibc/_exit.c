/*
 * _exit.c
 */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

#define __NR___exit __NR_exit

/* Syscalls can't return void... */
static inline _syscall1(int,__exit,int,rv);

__noreturn _exit(int rv)
{
  __exit(rv);
  for(;;);
}
