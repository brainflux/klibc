# -*- makefile -*-
#
# arch/i386/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

ARCHOBJS = \
	arch/$(ARCH)/exits.o \
	arch/$(ARCH)/socketcall.o \
	arch/$(ARCH)/setjmp.o \
	arch/$(ARCH)/libgcc/__ashldi3.o \
	arch/$(ARCH)/libgcc/__ashrdi3.o \
	arch/$(ARCH)/libgcc/__lshrdi3.o \
	arch/$(ARCH)/libgcc/__muldi3.o \
	arch/$(ARCH)/libgcc/__negdi2.o \
	arch/$(ARCH)/libgcc/__divdi3.o \
	arch/$(ARCH)/libgcc/__moddi3.o \
	arch/$(ARCH)/libgcc/__udivdi3.o \
	arch/$(ARCH)/libgcc/__umoddi3.o \
	arch/$(ARCH)/libgcc/__udivmoddi4.o

ARCHSOOBJS = $(patsubst %.o,%.lo,$(ARCHOBJS))

archclean:
