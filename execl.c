/*
 * execl.c
 */

#include <stdarg.h>
#include <unistd.h>

int execl(const char *path, const char *arg0, ...)
{
  va_list ap;
  int argc = 0;
  int size = 256;
  const char **argv;
  const char *arg;

  argv = malloc(size*sizeof(const char *));
  
  if ( !argv )
    return -1;

  va_start(ap, arg0);
  argv[argc++] = arg0;

  do {
    argv[argc++] = arg = va_arg(ap, const char *);
    if ( argc >= size ) {
      argv = realloc(argv, (size <<= 1)*sizeof(const char *));
      if ( !argv )
	return -1;
    }
  } while ( arg );

  va_end(args);

  return execve(path, argv, environ);
}


