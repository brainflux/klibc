#
# arch/alpha/syscall.S
#

#include <machine/asm.h>
	
	.text
	.align	3
	.type	__syscall_common,@function
	.ent	__syscall_common, 0
	.globl	__syscall_common
__syscall_common:
	callsys
	beq	a3, 1f
	ldah	a1, errno(zero)
	lda	v0, -1(zero)
	stq	a3, errno(a1)
1:		
	ret	zero,(ra),1

	.size	__syscall_common,.-__syscall_common
	.end	__syscall_common
