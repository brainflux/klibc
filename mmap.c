/*
 * mmap.c
 *
 * This is really just a syscall stub, but calls sys_mmap2 on
 * i386.
 */

#include <sys/syscall.h>
#include <sys/mman.h>

#ifdef __NR_mmap2
#undef __NR_mmap
#define __NR_mmap __NR_mmap2
#endif

_syscall6(void *,mmap,void *,start,size_t,length,int,prot,int,flags,int,fd,off_t,offset);
