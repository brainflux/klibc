#
# arch/ppc/crt0.S
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
	lwz	3,0(1)
	lwz	4,4(1)
	lwz	5,8(1)
	addi	1,1,-16
	lis	9,environ@ha
	stw	5,environ@l(9)
	bl	main
	bl	exit
	
	.size _start,.-_start
