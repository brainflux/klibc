/*
 * inet/inet_ntop.c
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in6.h>

const char *inet_ntop(int af, const void *cp, char *buf, size_t len)
{
  int xlen;

  switch ( af ) {
  case AF_INET:
    {
      uint32_t v = ((const struct in_addr *)cp)->s_addr;

      xlen = snprintf(buf, len, "%u.%u.%u.%u",
		      (v >> 24), (v >> 16) & 0xff, (v >> 8) & 0xff, v & 0xff);
    }
    break;

  case AF_INET6:
    {
      const struct in6_addr *s = (const struct in6_addr *)cp;
      
      xlen = snprintf(buf, len, "%x:%x:%x:%x:%x:%x:%x:%x",
		      ntohs(s->s6_addr16[0]), ntohs(s->s6_addr16[1]),
		      ntohs(s->s6_addr16[2]), ntohs(s->s6_addr16[3]),
		      ntohs(s->s6_addr16[4]), ntohs(s->s6_addr16[5]),
		      ntohs(s->s6_addr16[6]), ntohs(s->s6_addr16[7]));
    }
    break;

  default:
    errno = EAFNOSUPPORT;
    return NULL;
  }

  if ( xlen > len ) {
    errno = ENOSPC;
    return NULL;
  }

  return buf;
}

