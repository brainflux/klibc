/*
 * netinet/in.h
 */

#ifndef _NETINET_IN_H
#define _NETINET_IN_H

#include <stdint.h>
#include <endian.h>

#define htons(x)	__cpu_to_be16(x)
#define ntohs(x)	__be16_to_cpu(x)
#define htonl(x)	__cpu_to_be32(x)
#define ntohl(x)	__be32_to_cpu(x)

#endif /* _NETINET_IN_H */
