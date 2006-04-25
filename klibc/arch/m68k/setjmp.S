#
# arch/m68k/setjmp.S
#
# setjmp/longjmp for the m68k architecture
#

#
# The jmp_buf is assumed to contain the following, in order:
#	%d2..%d7
#	%a2..%a7
#	return address
#

	.text
	.align 2
	.globl setjmp
	.type setjmp, @function
setjmp:
	move.l	(%a7), %d0		| Return address
	move.l	4(%a7), %a0		| Buffer address
	movem.l	%d2-%d7/%a2-%a7, (%a0)
	move.l	%d0, 56(%a0)		| Return address
	moveq	#0, %d0
	moveq	#0, %a0			| Redundant return...
	rts

	.size setjmp,.-setjmp

	.text
	.align 2
	.globl longjmp
	.type longjmp, @function
longjmp:
	move.l	4(%a7), %a0		| Buffer address
	move.l	8(%a7), %d0		| Return value
	movem.l	(%a0), %d2-%d7/%a2-%a7,
	move.l	56(%a0), %a1
	move.l	%d0, %a0		| Redundant return...
	jmp.l	(%a1)

	.size longjmp,.-longjmp
