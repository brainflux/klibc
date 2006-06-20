# -*- makefile -*-
#
# arch/arm/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

KLIBCARCHOBJS = \
	arch/arm/setjmp.o \
	arch/arm/syscall.o \
	arch/arm/vfork.o \
	arch/arm/aeabi_nonsense.o \
	libgcc/__udivmodsi4.o \
	libgcc/__divdi3.o \
	libgcc/__moddi3.o \
	libgcc/__udivdi3.o \
	libgcc/__umoddi3.o \
	libgcc/__udivmoddi4.o \
	libgcc/__clzsi2.o \
	libgcc/__clzdi2.o \

