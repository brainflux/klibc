/*
 * arch/cris/syscall.S
 *
 * On cris, r9 contains the syscall number (set by generated stub);
 * r10..r13 contain arguments 0-3 per the standard calling convention,
 * and arguments 4-5 are passed in $mof and $srp; however, we have
 * to save $srp around the system call.
 */

	.set noreorder

	.text
	.subsection ".syscall"
	.align	4
	.globl	__syscall_common
	.type	__syscall_common,@function
__syscall_common:
	push	$srp
	move.d	[$sp+4],$mof
	move.d	[$sp+8],$srp
	break	13
	pop	$srp

	cmpq.d	-4096,$r10
	blt	1f
	  neg.d	$r10,$r11
	move.d	$r11,[errno]
	moveq.d	-1,$r10
1:
	ret
	  nop

	.size	__syscall_common,.-__syscall_common
