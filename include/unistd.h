/*
 * unistd.h
 */

#ifndef _UNISTD_H
#define _UNISTD_H

#include <extern.h>
#include <stddef.h>
#include <sys/types.h>

__extern __noreturn _exit(int);
__extern ssize_t read(int, void *, size_t);
__extern ssize_t write(int, const void *, size_t);

#endif /* _UNISTD_H */
