#
# arch/mips/crt0.S
#
# void _start(void)
# {
#    /* Divine up argc, argv, and envp */
#    environ = envp;
#    exit(main(argc, argv, envp));
# } 
#

#include <asm/regdef.h>
#include <asm/asm.h>

NESTED(__start, 32, 16)
	subu	$29, 32
	sw	zero, 16(sp)

	lw	a0, (sp)
	addiu	a1, sp, 4
	sll	a2, a0, 4
	addiu	a2, 4
	addiu	a2, a1
	sw	a2, environ
	jalr	main
	move	a0, v0
	jalr	exit

	END(__start)
