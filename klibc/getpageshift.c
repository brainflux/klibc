/*
 * getpageshift.c
 *
 * Return the current page size as a shift count
 */

#include <klibc/compiler.h>
#include <sys/syscall.h>
#include <unistd.h>

int __getpageshift(void)
{
  static int page_shift;
  int page_size;

  if ( __likely(page_shift) )
    return page_shift;

  page_size = getpagesize();

  while ( page_size > 1 ) {
    page_shift++;
    page_size >>= 1;
  }

  return page_shift;
}
