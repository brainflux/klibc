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
	.type _start,@function
	.globl _start
_start:
	ldr	r0, [sp], #4
	mov	r1, sp
	add	r2, r1, r0, lsl #2
	add	r2, r2, #4
	ldr	r3, =environ
	str	r2, [r3, #0]
	bl	main
	b	exit

	.size _start,.-_start
