/*
 * execvp.c
 */

#include <stdarg.h>
#include <unistd.h>

int execvp(const char *path, const char * const * argv)
{
  return execvpe(path, argv, environ);
}


