#
# crt0.S
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
	movl (%esp),%ecx
	leal 4(%esp),%edx
	leal 4(%edx,%ecx,4),%eax
	push %eax
	push %edx
	push %ecx
	movl %eax,environ
	call main
	push %eax
	call exit
	
	.size _start,.-_start
