#
# exit and _exit get included in *every* program, and gcc generates
# horrible code for them.  Yes, this only saves a few bytes, but
# it does it in every program.
#

#include <asm/unistd.h>

#ifdef __i386__

	.data
	.align 4
	.globl __exit_handler
	.type __exit_handler,@object
__exit_handler:
	.long _exit
	.size __exit_handler,4

	.text
	.align 4
	.globl exit
	.type exit,@function
exit:
	jmp *(__exit_handler)
	.size exit,.-exit

	/* No need to save any registers... we're exiting! */
	.text
	.align 4
	.globl _exit
	.type _exit,@function
_exit:
#if __NR_exit == 1
	popl %ebx
	xorl %eax,%eax
	popl %ebx
	incl %eax
#else
	movl 4(%esp),%ebx
	movl $__NR_exit,%eax
#endif
	int $0x80
	hlt
	.size _exit,.-exit

#elif defined(__x86_64__)

	.data
	.align 8
	.globl __exit_handler
	.type __exit_handler,@object
__exit_handler:
	.quad _exit
	.size __exit_handler,8

	.text
	.align 8
	.globl exit
	.type exit,@function
exit:
	jmp *(__exit_handler)
	.size exit,.-exit

	/* No need to save any registers... we're exiting! */
	.text
	.align 4
	.globl _exit
	.type _exit,@function
_exit:
	movl $__NR_exit,%rax
	/* The argument is already in %rdi */
	syscall
	.size _exit,.-exit

#endif
