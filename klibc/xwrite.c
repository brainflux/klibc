/*
 * xwrite.c
 */

#include <errno.h>
#include <unistd.h>
#include <xio.h>

ssize_t __xwrite(int fd, const void *buf, size_t count)
{
  ssize_t bytes = 0;
  ssize_t rv;
  const char *p = buf;

  while ( count ) {
    rv = write(fd, p, count);
    if ( rv == -1 ) {
      if ( errno == EINTR )
	continue;
      else
	return bytes ? bytes : -1;
    } else if ( rv == 0 ) {
      break;
    }

    p += rv;
    bytes += rv;
    count -= rv;
  }

  return bytes;
}

    
      
