/*
 * arch/mips/include/klibc/endian.h
 */

#ifndef _KLIBC_ENDIAN_H
#define _KLIBC_ENDIAN_H

#if defined(__MIPSEB__)
# define __BYTE_ORDER __BIG_ENDIAN
#elif defined(__MIPSEL__)
# define __BYTE_ORDER __LITTLE_ENDIAN
#else
# error "Neither __MIPSEB__ nor __MIPSEL__ defined"
#endif

#endif /* _KLIBC_ENDIAN_H */
