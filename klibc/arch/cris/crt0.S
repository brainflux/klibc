#
# arch/cris/crt0.S
#
# Does arch-specific initialization and invokes __libc_init
# with the appropriate arguments.
#
# See __static_init.c or __shared_init.c for the expected
# arguments.
#

	.text
	.balign 4
	.type	__start,@function
	.globl	__start
__start:
	/* Save the address of the ELF argument array */
	move.d	$sp,$r10	/* Address of ELF arguments */

	/* atexit() function (assume null) */
	moveq	0,$r11
	
	/* Set up a dummy stack frame to keep gcc from getting confused */
	push	$r11
	jump	___libc_init
	  push	$r11

	.size __start, .-__start

