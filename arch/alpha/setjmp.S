#
# setjmp.S
#

#
# The jmp_buf looks like:
#
#	s0..5
#	fp
#	ra
#	gp
#	sp
#

#include <machine/asm.h>
	
	.text
	.align	3
	.type	setjmp,@function
	.ent	setjmp, 0
	.globl	setjmp
setjmp:
	lda	v0,  0(zero)
	stq	s0,  0(a0)
	stq	s1,  8(a0)
	stq	s2, 16(a0)
	stq	s3, 24(a0)
	stq	s4, 32(a0)
	stq	s5, 40(a0)
	stq	fp, 48(a0)
	stq	ra, 56(a0)
	stq	gp, 64(a0)
	stq	sp, 72(a0)
	ret	zero,(ra),1

	.size setjmp,.-setjmp
	.end setjmp

	.type	longjmp,@function
	.ent	longjmp, 0
	.globl	longjmp
longjmp:
	mov	a1, v0
	ldq	s0,  0(a0)
	ldq	s1,  8(a0)
	ldq	s2, 16(a0)
	ldq	s3, 24(a0)
	ldq	s4, 32(a0)
	ldq	s5, 40(a0)
	ldq	fp, 48(a0)
	ldq	ra, 56(a0)
	ldq	gp, 64(a0)
	ldq	sp, 72(a0)
	/* We're bound to get a mispredict here, but at least give us
	   a chance to get the return stack back in sync... */
	ret	zero,(ra),1

	.size longjmp,.-longjmp
	.end longjmp


