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
	.align 3
	.type _start,@function
	.ent _start, 0
	.globl _start
_start:
	.frame  $15, 0, $31
	mov	$31, $15
	br	$29, 1f
1:	ldgp	$29, 0($29)
	subq	$30, 16, $30
	.prologue 0

	ldl	$16, 16($30)
	lda	$17, 24($30)
	addq	$16, 1, $18
	s8addq	$18, $17, $18
	jsr	$26, main

	mov	$0, $16
	jsr	$26, exit
	
	.size _start,.-_start

	.weak __start
__start	= _start