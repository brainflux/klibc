#
# arch/s390x/crt0.S
#
# void _start(void)
# {
#    /* Divine up argc, argv, and envp */
#    environ = envp;
#    exit(main(argc, argv, envp));
# } 
#

	.text
	.align 8
	.type _start,@function
	.globl _start
_start:
				# save argc
	lg	%r2,0(%r15)
				# save argv
	la	%r3,8(%r15)
				# compute envp
	sllg	%r4,%r2,3
	la	%r4,8(%r4,%r3)

				# create stack frame
	aghi	%r15,-160

				# save global environ
	larl	%r1,environ
	stg	%r4,0(%r1)
				# call main
	brasl	%r14,main
				# call exit
	brasl	%r14,exit

	.size _start,.-_start
