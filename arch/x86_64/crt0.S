#
# arch/x86_64/crt0.S
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
	movq (%rsp),%rdi
	leaq 8(%rsp),%rsi
	leaq 8(%rsi,%rdi,8),%rdx
	movq %rdx,environ
	call main
	movq %rdi,%rax
	call exit
	
	.size _start,.-_start
