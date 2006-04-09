/*
 * klibc/sysconfig.h
 *
 * Allows for definitions of some things which may be system-dependent
 * NOTE: this file must not result in any output from the preprocessor.
 */

#ifndef _KLIBC_SYSCONFIG_H
#define _KLIBC_SYSCONFIG_H

#include <klibc/archconfig.h>
#include <asm/unistd.h>

/*
 * These are the variables that can be defined in <klibc/archconfig.h>.
 * For boolean options, #define to 0 to disable, #define to 1 to enable.
 *
 * If undefined, they will be given defaults here.
 */


/*
 * _KLIBC_NO_MMU:
 *
 *	Indicates this architecture doesn't have an MMU, and therefore
 *	has no mmap(), munmap(), mremap(), msync(), mprotect(), or any
 *	of the mlock family.
 */
/* Default to having an MMU if we can find any mmap system call */
#ifndef _KLIBC_NO_MMU
# if defined(__NR_mmap) || defined(__NR_mmap2)
#  define _KLIBC_NO_MMU 0
# else
#  define _KLIBC_NO_MMU 1
# endif
#endif


/*
 * _KLIBC_USE_MMAP2:
 *
 *	Indicates that this architecture should use sys_mmap2 instead
 *	of sys_mmap.  This is the default on 32-bit architectures, assuming
 *	sys_mmap2 exists.
 */
#ifndef _KLIBC_USE_MMAP2
# if (_BITSIZE == 32 && defined(__NR_mmap2)) || \
     (_BITSIZE == 64 && !defined(__NR_mmap))
#  define _KLIBC_USE_MMAP2 1
# else
#  define _KLIBC_USE_MMAP2 0
# endif
#endif

/*
 * _KLIBC_MMAP2_SHIFT:
 *
 *	Indicate the shift of the offset parameter in sys_mmap2.
 *	On most architectures, this is always 12, but on some
 *	architectures it can be a different number, or the current
 *	page size.  If this is dependent on the page size, define
 *	this to an expression which includes __getpageshift().
 */
#ifndef _KLIBC_MMAP2_SHIFT
# define _KLIBC_MMAP2_SHIFT 12
#endif

/*
 * _KLIBC_MALLOC_USES_SBRK:
 *
 *	Indicates that malloc() should use sbrk() to obtain raw memory
 *	from the system, rather than mmap().  This is the default if
 *	_KLIBC_NO_MMU is enabled.
 */
/* Use sbrk if we have no MMU, otherwise mmap */
#ifndef _KLIBC_MALLOC_USES_SBRK
# define _KLIBC_MALLOC_USES_SBRK _KLIBC_NO_MMU
#endif


/*
 * _KLIBC_MALLOC_CHUNK_SIZE:
 *	This is the minimum chunk size we will ask the kernel for using
 *	malloc(); this should be a multiple of the page size and must
 *	be a power of 2.
 */
#ifndef _KLIBC_MALLOC_CHUNK_SIZE
# define _KLIBC_MALLOC_CHUNK_SIZE	65536
#endif


/*
 * _KLIBC_SBRK_ALIGNMENT:
 *
 *	This is the minimum alignment for the memory returned by
 *	sbrk().  It must be a power of 2.  If _KLIBC_MALLOC_USES_SBRK
 *	is set it should be no smaller than the size of struct
 *	arena_header in malloc.h (== 4 pointers.)
 */
#ifndef _KLIBC_SBRK_ALIGNMENT
# define _KLIBC_SBRK_ALIGNMENT		32
#endif


/*
 * _KLIBC_USE_RT_SIG:
 *
 *      Indicates that this architecture should use the rt_sig*()
 *      family of system calls, even if the older system calls are
 *      provided.  This requires that <asm/signal.h> is correct for
 *      using with the rt_sig*() system calls.  This is the default if
 *      the older system calls are undefined in <asm/unistd.h>.
 *
 */
#ifndef _KLIBC_USE_RT_SIG
# ifdef __NR_sigaction
#  define _KLIBC_USE_RT_SIG 0
# else
#  define _KLIBC_USE_RT_SIG 1
# endif
#endif


/*
 * _KLIBC_NEEDS_SA_RESTORER:
 *
 *	Some architectures, like x86-64 and some i386 Fedora kernels,
 *	do not provide a default sigreturn, and therefore must have
 *	SA_RESTORER set.
 */
#ifndef _KLIBC_NEEDS_SA_RESTORER
# define _KLIBC_NEEDS_SA_RESTORER 0
#endif

#endif /* _KLIBC_SYSCONFIG_H */
