	.file "exits.S"

#
# exit and _exit get included in *every* program, and gcc generates
# horrible code for them.  Yes, this only saves a few bytes, but
# it does it in every program.
#

#include <asm/unistd.h>

#ifdef __i386__

	.data
	.align 4
	.type __exit_handler,@object
	.size __exit_handler,4
__exit_handler:
	.long _exit

	.text
	.globl exit
	.type exit,@function
exit:
	jmp (__exit_handler)
	.size exit,.-exit


	/* No need to save any registers... we're exiting! */
	.globl _exit
_exit:
	movl 4(%esp),%ebx
	movl $__NR_exit,%eax
	int $0x80
	.size _exit,.-exit

#endif

