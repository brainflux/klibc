#
# arch/alpha/sysdual.S
#

#
# Some system calls have an alternate return value in r20 (a4).
# This system call stub is for system calls where that is
# the "real" return value.
#

#include <machine/asm.h>
	
	.text
	.align	3
	.type	__syscall_dual1,@function
	.ent	__syscall_dual1, 0
	.globl	__syscall_dual1
__syscall_dual1:
	callsys
	mov	v0, a4
	ldgp	gp, 0(pv)
	beq	a3, 1f
	lda	a1, errno
	lda	v0, -1(zero)
	std	a3, 0(a1)
1:
	ret	zero,(ra),1

	.size	__syscall_dual1,.-__syscall_dual1
	.end	__syscall_dual1
