/*
 * memset.c
 */

#include <string.h>

void *memset(void *dst, int c, size_t n)
{
  char *q = dst;

#if defined(__i386__)
  asm volatile("cld ; rep ; stosl ; movl %3,%0 ; rep ; stosb"
	       : "+c" (n >> 2), "+D" (q)
	       : "a" ((unsigned char)c * 0x01010101U), "r" (n & 3));
#elif defined(__x86_64__)
  asm volatile("cld ; rep ; stosq ; movl %3,%%ecx ; rep ; stosb"
	       : "+c" (n >> 3), "+D" (q)
	       : "a" ((unsigned char)c * 0x0101010101010101U,
		      "r" ((uint32_t)n & 7));
#else
  while ( n-- ) {
    *q++ = c;
  }
#endif

  return dst;
}
