/*
 * klibc/compiler.h
 *
 * Various compiler features
 */

#ifndef _KLIBC_COMPILER_H
#define _KLIBC_COMPILER_H

/* How to declare a function that is called/callable from assembly */
#ifdef __i386__
# ifdef __GNUC__
#  define __asmlinkage __attribute__((cdecl))
# else
#  define __asmlinkage __cdecl
# endif
#endif

/* How to declare a function that *must* be inlined */
#ifdef __GNUC__
# if __GNUC_MAJOR__ >= 3
#  define __must_inline static __inline__ __attribute__((always_inline))
# else
#  define __must_inline extern __inline__
# endif
#else
# define __must_inline inline	/* Just hope this works... */
#endif

/* How to declare a function that does not return */
#ifdef __GNUC__
# define __noreturn void __attribute__((noreturn))
#else
# define __noreturn void
#endif

/* How to declare a "constant" function (a function in the
   mathematical sense) */
#ifdef __GNUC__
# define __constfunc __attribute__((const))
#else
# define __constfunc
#endif

/* Format attribute */
#ifdef __GNUC__
# define __formatfunc(t,f,a) __attribute__((format(t,f,a)))
#else
# define __formatfunc(t,f,a)
#endif

#endif
