#
# arch/ppc/crt0.S
#

	.text
	.align 4
	.type _start,@function
	.globl _start
_start:
	mr	3,1
	li	4,0
	bl	__libc_init
	
	.size _start,.-_start
