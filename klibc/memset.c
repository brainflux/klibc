/*
 * memset.c
 */

#include <string.h>

void *memset(void *dst, int c, size_t n)
{
  char *q = dst;

#if defined(__i386__)
  asm volatile("cld ; movl %0,%%edx ; shrl $2,%0 ; rep ; stosl ; "
	       "movl %%edx,%0 ; andl $3,%0 ; rep ; stosb"
	       : "+c" (n), "+D" (q)
	       : "a" ((unsigned char)c * 0x01010101U)
	       : "edx");
#elif defined(__x86_64__)
  asm volatile("cld ; movl %0,%%edx ; shrq $3,%0 ; rep ; stosq ; "
	       "movl %%edx,%0 ; andl $7,%0 ; rep ; stosb"
	       : "+c" (n), "+D" (q)
	       : "a" ((unsigned char)c * 0x0101010101010101U)
	       : "rdx");
#else
  while ( n-- ) {
    *q++ = c;
  }
#endif

  return dst;
}
