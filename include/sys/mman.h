/*
 * sys/mman.h
 */

#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H

#include <extern.h>
#include <sys/types.h>
#include <linux/mman.h>

__extern void *mmap(void *, size_t, int, int, int, off_t);
__extern int munmap(void *, size_t);
__extern void *mremap(void *, size_t, size_t, unsigned long);

#endif /* _SYS_MMAN_H */
