/*
 * stddef.h
 */

#ifndef _STDDEF_H
#define _STDDEF_H

#define __KLIBC__ 1

#include <bitsize/stddef.h>

#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif

#define offsetof(t,m) ((size_t)&((t *)0->m))

#ifdef __GNUC__
# define __noreturn void __attribute__((noreturn))
#else
# define __noreturn void
#endif

#endif /* _STDDEF_H */
