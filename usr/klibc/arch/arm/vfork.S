/*
 * arch/arm/vfork.S
 *
 * vfork - nasty system call which must not use the stack.
 */

#include <asm/unistd.h>
	
	.type	vfork,#function
	.globl	vfork
#ifndef __thumb__
	/* ARM version - this is executed after the swi */

	.align	4
vfork:
	swi	# __NR_vfork
        cmn     r0, #4096
        rsbcs	r2, r0, #0
        ldrcs	r3, 1f
        mvncs	r0, #0
        strcs	r2, [r3]
	mov	pc, lr

	.align 4
1:
	.word	errno

#else
	/* Thumb version - must still load r4 and r5 and run swi */

	.thumb_func
	.align	2
vfork:
	mov	r7, # __NR_vfork
	swi	0
	ldr	r1, 2f
	cmp	r0, r1
	bcc	1f
	ldr	r1, 3f
	neg	r2, r0
	mvn	r0, #0
	str	r2, [r1]
1:
	mov	pc, lr

	.align	4
2:
	.word	#-4095
3:
	.word	errno

#endif
