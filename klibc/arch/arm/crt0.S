#
# arch/arm/crt0.S
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
	.type _start,#function
	.globl _start

#ifdef __thumb__

/*
 * Thumb version
 */

	.thumb_func
_start:	pop	{r0}
	mov	r1, sp
	ldr	r3, =environ
	lsl	r2, r0, #2
	add	r2, r1, r2
	add	r2, r2, #4
	str	r2, [r3, #0]
	bl	main
	bl	exit

#else

/*
 * ARM version 
 */

_start: ldr	r0, [sp], #4
	mov	r1, sp
	ldr	r3, =environ
	add	r2, r1, r0, lsl #2
	add	r2, r2, #4
	str	r2, [r3, #0]
	bl	main
	b	exit

#endif

	.size _start,.-_start
