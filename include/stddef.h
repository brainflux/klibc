/*
 * stddef.h
 */

#ifndef _STDDEF_H
#define _STDDEF_H

#include <bitsize/stddef.h>

#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif

#define offsetof(t,m) ((size_t)&((t *)0->m))

#endif /* _STDDEF_H */
