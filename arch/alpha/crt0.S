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
	.frame fp, 0, zero
	mov	zero, fp
	br	gp, 1f
1:	ldgp	gp, 0(gp)
	subq	sp, 16, sp
	.prologue 0

	ldl	a0, 16(sp)
	lda	a1, 24(sp)
	addq	a0, 1, a2
	s8addq	a2, a1, a2
	jsr	ra, main

	mov	v0, a0
	jsr	ra, exit
	
	.size _start,.-_start
