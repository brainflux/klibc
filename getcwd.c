/*
 * getcwd.c
 */

#include <unistd.h>
#include <sys/syscall.h>

#define __NR__getcwd __NR_getcwd
static inline _syscall2(int,_getcwd,char *,buf,size_t,size);

char *getcwd(char *buf, size_t size)
{
  return ( _getcwd(buf, size) < 0 ) ? NULL : buf;
}

