/*
 * exit.c
 */

#include <extern.h>
#include <stdlib.h>

#include "atexit.h"

struct atexit *__atexit_list;

__noreturn exit(int rv)
{
  struct atexit *ap;

  for ( ap = __atexit_list ; ap ; ap = ap->next ) {
    ap->fctn(rv, ap->arg);	/* This assumes extra args are harmless */
  }

  _exit(rv);
}

  
  
