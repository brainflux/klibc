/*
 * memcpy.c
 */

#include <string.h>

void *memcpy(void *dst, const void *src, size_t n)
{
  const char *p = src;
  char *q = dst;
#if defined(__i386__)
  asm volatile("cld ; rep ; movsl ; movl %3,%0 ; rep ; movsb"
	       : "+c" (n >> 2), "+S" (p), "+D" (q)
	       : "r" (n & 3));
#elif defined(__x86_64__)
  asm volatile("cld ; rep ; movsq ; movl %3,%%ecx ; rep ; movsb"
	       : "+c" (n), "+S" (p), "+D" (q)
	       : "r" ((uint32_t)n & 7));
#else
  while ( n-- ) {
    *q++ = *p++;
  }
#endif

  return dst;
}
