/*
 * arch/i386/syscall.S
 *
 * long __syscall(int nr, ...);
 *
 * Note: because this is a varadic function, REGPARM does not apply.
 */

#define ARG(n) (n*4+24)(%esp)

	.text
	.align	4
	.globl	__syscall
	.type	__syscall,@function
__syscall:
	pushl	%ebx
	pushl	%esi
	pushl	%edi
	pushl	%ebp

	movl	ARG(-1),%eax		# Syscall number
	movl	ARG(0),%ebx		# Syscall arguments
	movl	ARG(1),%ecx
	movl	ARG(2),%edx
	movl	ARG(3),%esi
	movl	ARG(4),%edi
	movl	ARG(5),%ebp
	int	$0x80

	cmpl	$-4096,%eax

	popl	%ebp
	popl	%edi
	popl	%esi
	popl	%ebx

	jb	1f

	# Error return, must set errno
	negl	%eax
	movl	%eax,errno
	orl	$-1,%eax		# orl $-1 smaller than movl $-1

1:
	ret

	.size	__syscall,.-__syscall
