#
# arch/cris/setjmp.S
#
# setjmp/longjmp for the cris architecture
#

#
# The jmp_buf is assumed to contain the following, in order:
#	$r8..$r0	(in that order)
#	$sp	($r14)
#	return address
#

	.set noreorder
	
	.text
	.align 4
	.globl setjmp
	.type setjmp, @function
setjmp:
	movem	$r8,[$r10+]		# Save $r8..$r0 at $r10..
	move.d	$sp,[$r10+]
	ret
	  move	$srp,[$r10+]

	.size setjmp,.-setjmp

	.text
	.align 4
	.globl longjmp
	.type longjmp, @function
longjmp:
	movem	[$r10+],$r8		# Load $r8..$r0 from $r10..
	move.d	[$r10+],$sp
	jump	[$r10+]
	  move.d $r11,$r10
	
	.size longjmp,.-longjmp
