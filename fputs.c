/*
 * fputs.c
 *
 * This isn't quite fputs() in the stdio sense, since we don't
 * have stdio, but it takes a file descriptor argument instead
 * of the FILE *.
 */

#include <stdio.h>
#include <string.h>
#include <xio.h>

int fputs(const char *s, int fd)
{
  return __xwrite(fd, s, strlen(s));
}
