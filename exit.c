/*
 * exit.c
 */

#include <extern.h>
#include <stdlib.h>
#include <unistd.h>

/* This allows atexit/on_exit to install a hook */
__noreturn (*__exit_handler)(int) = _exit;

__noreturn exit(int rv)
{
  __exit_handler(rv);
}
