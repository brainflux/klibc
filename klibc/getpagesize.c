/*
 * getpagesize.c
 */

#include <klibc/compiler.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/sysinfo.h>

int getpagesize(void)
{
  static int page_size;
  struct sysinfo si;
  int rv;

  if ( __likely(page_size) )
    return page_size;

  rv = sysinfo(&si);
  if ( rv == -1 )
    return -1;

  return (page_size = si.mem_unit);
}
