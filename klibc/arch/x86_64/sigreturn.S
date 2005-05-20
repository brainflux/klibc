/*
 * arch/x86_64/sigreturn.S
 */

#include <asm/unistd.h>
	
	.text
	.align	4
	.globl	__syscall_common
	.type	__syscall_common,@function
__sigreturn:
	movl	$__NR_rt_sigreturn,%eax
	syscall

	.size	__sigreturn,.-__sigreturn
