/*
 * inet/inet_ntoa.c
 */

#include <arpa/inet.h>
#include <stdio.h>

char *inet_ntoa(struct in_addr addr)
{
  static char name[16];
  uint32_t v = addr.s_addr;

  sprintf(name, "%u.%u.%u.%u",
	  (v >> 24), (v >> 16) & 0xff, (v >> 8) & 0xff, v & 0xff);
  return name;
}
