/*
 * fcntl.h
 */

#ifndef _FCNTL_H
#define _FCNTL_H

#include <klibc/compiler.h>
#include <sys/types.h>
#include <linux/fcntl.h>

/* This is defined here as well as in <unistd.h> since old-style code
   would still include <fcntl.h> when using open(), and open() being
   a varadic function changes its calling convention on some architectures. */
#ifndef __IN_OPEN_C
__extern __cdecl int open(const char *, int, ...);
#endif

#endif /* _FCNTL_H */
