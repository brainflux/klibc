#
# arch/s390/crt0.S
#
# void _start(void)
# {
#    /* Divine up argc, argv, and envp */
#    environ = envp;
#    exit(main(argc, argv, envp));
# } 
#

	.text
	.align 4
	.type _start,@function
	.globl _start
_start:
				# save argc 
	l	%r2,0(%r15)
				# save argv
	la	%r3,4(%r15)
				# compute envp
	lr	%r4,%r2
	sll	%r4,2
	la	%r4,4(%r4,%r3)

				# literal pool
	bras	%r13,.LTN0_0
.LT0_0:
	.long	environ
	.long	main
	.long	exit	
.LTN0_0:
				# create stack frame
	ahi	%r15,-96

				# save global environ
	l	%r1,0(%r13)
	st	%r4,0(%r1)
				# call main
	l	%r1,4(%r13)
	basr	%r14,%r1
				# call exit
	l	%r1,8(%r13)
	basr	%r14,%r1

	.size _start,.-_start
