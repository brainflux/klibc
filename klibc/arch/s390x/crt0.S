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
	lgr	%r2,%r15
	lghi	%r3,0
	aghi	%r15,-160
	jg	__libc_init

	.size _start,.-_start
