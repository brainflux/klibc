/*
 * arch/i386/open.S
 *
 * Handle the open() system call - oddball due to the varadic
 * prototype, which forces the use of the cdecl calling convention.
 */

#include <asm/unistd.h>

	.globl	open
	.type	open,@function

open:
#ifdef REGPARM
	movl	4(%esp),%eax
	movl	8(%esp),%edx
	movl	12(%esp),%ecx
#endif
	pushl	$__NR_open
	jmp	__syscall_common

	.size	open,.-open
