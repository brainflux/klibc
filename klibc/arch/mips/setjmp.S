#
# arch/mips/setjmp.S
#
# setjmp/longjmp for the MIPS architecture
#
# The jmp_buf is assumed to contain the following, in order:
#	s0..s7
#	gp
#	sp
#	s8
#	ra
#	f20..f30
#	fcr31
#

#include <machine/asm.h>
	
LEAF(setjmp)
	sw	s0,  0(a0)
	sw	s1,  4(a0)
	sw	s2,  8(a0)
	sw	s3, 12(a0)
	sw	s4, 16(a0)
	sw	s5, 20(a0)
	sw	s6, 24(a0)
	sw	s7, 28(a0)
	sw	gp, 32(a0)
	sw	sp, 36(a0)
	sw	s8, 40(a0)
	swc1	$f20,48(a0)
	swc1	$f21,52(a0)
	swc1	$f22,56(a0)
	swc1	$f23,60(a0)
	swc1	$f24,64(a0)
	swc1	$f25,68(a0)
	swc1	$f26,72(a0)
	swc1	$f27,76(a0)
	swc1	$f28,80(a0)
	swc1	$f29,84(a0)
	swc1	$f30,88(a0)
	swc1	$fcr31,92(a0)
	jr	ra
	 sw	ra, 44(a0)
	
	END(setjmp)

LEAF(longjmp)
	lw	ra, 44(a0)	# Give time to settle...
	lw	s0,  0(a0)
	lw	s1,  4(a0)
	lw	s2,  8(a0)
	lw	s3, 12(a0)
	lw	s4, 16(a0)
	lw	s5, 20(a0)
	lw	s6, 24(a0)
	lw	s7, 28(a0)
	lw	gp, 32(a0)
	lw	sp, 36(a0)
	lw	s8, 40(a0)
	lwc1	$f20,48(a0)
	lwc1	$f21,52(a0)
	lwc1	$f22,56(a0)
	lwc1	$f23,60(a0)
	lwc1	$f24,64(a0)
	lwc1	$f25,68(a0)
	lwc1	$f26,72(a0)
	lwc1	$f27,76(a0)
	lwc1	$f28,80(a0)
	lwc1	$f29,84(a0)
	lwc1	$f30,88(a0)
	lwc1	$fcr31,92(a0)
	jr	ra
	 move	a1, v0

	END(longjmp)
	
	