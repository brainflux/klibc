!
! setjmp.S
!
! Basic setjmp/longjmp
!
! This code was based on the equivalent code in NetBSD
!

#include <machine/asm.h>

!
! The jmp_buf contains the following entries:
!   sp
!   fp
!   pc
!	
ENTRY(setjmp)
	st	%sp,[%o0+0]	! Callers stack pointer
	st	%o7,[%o0+4]	! Return pc
	st	%fp,[%o0+8]	! Frame pointer
	retl			! Return
	 clr	%o0		!  ...0

ENTRY(longjmp)
	mov	%o0,%g6		! Save desired frame in a global register
	mov	%o1,%g1		! Save return value in a global register
	ld	[%g6+4],%g7	! Get callers frame
1:
	cmp	%fp,%g7		! Desired frame?
	bl,a	1b		! Below...
	 restore		!  pop the frame and loop
	bne,a	.L_botch	! If there...
	 ld	[%g6+0],%o2	!  fetch return %sp

	cmp	%o2,%sp		! %sp must not decrease
	bl,a	.L_botch
	 mov	%o2,%sp		! If ok, put in place

	ld	[%g6+8],%o3	! Fetch return pc
	jmp	%o3 + 8		! success; return %g1
	 mov	%g1,%o0

.L_botch:
	unimp 0			! Botched longjmp
