#
# arch/cris/crt0.S
#
# Does arch-specific initialization and invokes __libc_init
# with the appropriate arguments.
#
# See __static_init.c or __shared_init.c for the expected
# arguments.
#

	.set noreorder
		
	.text
	.balign 4
	.type	_start,@function
	.globl	_start
_start:
	# Set up a dummy stack frame to keep gcc from getting confused
	push	$p8		# p8 == 0
	push	$p8		# p8 == 0
	
	# Save the address of the ELF argument array
	move.d	$sp,$r10	# Address of ELF arguments

	jump	__libc_init
	  moveq.d	0,$r11	# atexit() function [assume null]

	.size _start, .-_start

