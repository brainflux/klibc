#
# exit and _exit get included in *every* program, and gcc generates
# horrible code for them.  Yes, this only saves a few bytes, but
# it does it in every program.
#

#include <asm/unistd.h>

	.data
	.align 4
	.globl __exit_handler
	.type __exit_handler,#object
__exit_handler:
	.long _exit
	.size __exit_handler,4

	.text
	.align 4
	.globl exit
	.type exit,#function
	.thumb_func
exit:
	ldr	r3, =__exit_handler
	ldr	r3, [r3, #0]
	mov	pc, r3
	.size exit,.-exit

	/* No need to save any registers... we're exiting! */
	.globl _exit
	.type _exit,#function
	.thumb_func
_exit:	mov	r7, #__NR_exit
	swi	0
	b	_exit
	.size _exit,.-exit
