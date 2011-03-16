#
# arch/ppc64/crt0.S
#
# void _start(void)
# {
#    /* Divine up argc, argv, and envp */
#    environ = envp;
#    exit(main(argc, argv, envp));
# }
#

	.section ".toc","aw"
.LC0:	.tc	environ[TC],environ

	.text
	.align 4

	.section ".opd","aw"
_start:
	.quad	._start, .TOC.@tocbase, 0
	.previous
	.size	_start, 24
	.type	._start,@function
	.globl	_start
	.globl	._start
._start:
	stdu    %r1,-32(%r1)
	addi    %r3,%r1,32
	li	%r4,0		/* fini (unused) */
	b 	.__libc_init
	nop

	.size ._start,.-._start
