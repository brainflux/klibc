#
# arch/alpha/crt0.S
#
# void _start(void)
# {
#    /* Divine up argc, argv, and envp */
#    environ = envp;
#    exit(main(argc, argv, envp));
# } 
#

	.text
	.align	5
	.type	_start,@function
	.ent	_start, 0
	.globl	_start
_start:
	.frame  $30, 0, $26, 0
	.mask	0x4000000,-16
	mov	$31, $15
	br	$29, 1f
1:	ldgp	$29, 0($29)
	.prologue 0

	lda	$1, environ
	ldl	$16, 0($30)
	lda	$17, 8($30)
	addq	$16, 1, $18
	s8addq	$18, $17, $18
	stq	$18, 0($1)
	jsr	$26, main

	mov	$0, $16
	jsr	$26, exit
	
	.size _start,.-_start

	.weak __start
__start	= _start