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
	lr	%r2,%r15
	lhi	%r3,0
	ahi	%r15,-96
	bras	%r1,.L0
.L0:
	l	%r1,.L1-.L0(%r1)
	br	%r1
.L1:
	.long	__libc_init

	.size _start,.-_start
