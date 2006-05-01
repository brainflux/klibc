/*
 * vfork.c
 *
 * Emulate vfork() with fork() if necessary
 */

#include <unistd.h>
#include <klibc/compiler.h>
#include <klibc/sysconfig.h>

#if !_KLIBC_NO_MMU && !_KLIBC_REAL_VFORK
__ALIAS(int, vfork, (void), fork)
#endif
