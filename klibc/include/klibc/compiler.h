/*
 * klibc/compiler.h
 *
 * Various compiler features
 */

#ifndef _KLIBC_COMPILER_H
#define _KLIBC_COMPILER_H

#ifdef __GNUC__
# if __GNUC_MAJOR__ >= 3
#  define __must_inline static __inline__ __attribute__((always_inline))
# else
#  define __must_inline extern __inline__
#else
# define __must_inline inline	/* Just hope this works... */
#endif

#ifdef __GNUC__
# define __noreturn void __attribute__((noreturn))
#else
# define __noreturn void
#endif

#endif
