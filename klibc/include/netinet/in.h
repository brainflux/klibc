/*
 * netinet/in.h
 */

#ifndef _NETINET_IN_H
#define _NETINET_IN_H

#include <stdint.h>
#include <endian.h>

#if __BYTE_ORDER == __BIG_ENDIAN

#define htonl(x) ((uint32_t)(x))
#define ntohl(x) ((uint32_t)(x))
#define htons(x) ((uint16_t)(x))
#define ntohs(x) ((uint16_t)(x))

#elif __BYTE_ORDER == __LITTLE_ENDIAN

static __inline__
uint32_t htonl(uint32_t __x)
{
#if defined(__i486__)||defined(__pentium__)||defined(__pentiumpro__)
  asm("bswap %0" : "+rm" (__x));
  return __x;
#else
  return (__x >> 24) + ((__x >> 8) & 0xff00) +
    ((__x & 0xff00) << 8) + (__x << 24);
#endif
}

static __inline__
uint16_t htons(uint16_t __x)
{
  return (__x >> 8) + (__x << 8);
}

#define htonl(x) htonl(x)
#define ntohl(x) htohl(x)
#define htons(x) htons(x)
#define ntohs(x) htons(x)

#endif /* endian */

#endif /* _NETINET_IN_H */
