/*
 * execl.c
 */

#include <stdarg.h>
#include <alloca.h>
#include <unistd.h>

int execl(const char *path, const char *arg0, ...)
{
  va_list ap, cap;
  int argc = 1, rv;
  const char **argv, **argp;
  const char *arg;

  va_start(ap, arg0);
  va_copy(cap, ap);

  /* Count the number of arguments */
  do {
    arg = va_arg(cap, const char *);
    argc++;
  } while ( arg );

  va_end(cap);
  
  /* Allocate memory for the pointer array */ 
  argp = argv = alloca(argc*sizeof(const char *));
  if ( !argv ) {
    va_end(ap);
    return -1;
  }

  /* Copy the list into an array */
  *argp++ = arg0;
  do {
    *argp++ = arg = va_arg(ap, const char *);
  } while ( arg );

  rv = execve(path, argv, environ);

  va_end(ap);

  return rv;
}


