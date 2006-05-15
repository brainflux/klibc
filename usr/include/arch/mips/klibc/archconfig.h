/*
 * include/arch/mips/klibc/archconfig.h
 *
 * See include/klibc/sysconfig.h for the options that can be set in
 * this file.
 *
 */

#ifndef _KLIBC_ARCHCONFIG_H
#define _KLIBC_ARCHCONFIG_H

/* MIPS has architecture-specific code for vfork() */
#define _KLIBC_REAL_VFORK 1

#endif				/* _KLIBC_ARCHCONFIG_H */
