# -*- makefile -*-
#
# arch/i386/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

KLIBCARCHOBJS = \
	arch/$(KLIBCARCH)/socketcall.o \
	arch/$(KLIBCARCH)/setjmp.o \
	arch/$(KLIBCARCH)/syscall.o \
	arch/$(KLIBCARCH)/varsyscall.o \
	arch/$(KLIBCARCH)/open.o \
	arch/$(KLIBCARCH)/openat.o \
	arch/$(KLIBCARCH)/sigreturn.o \
	arch/$(KLIBCARCH)/libgcc/__ashldi3.o \
	arch/$(KLIBCARCH)/libgcc/__ashrdi3.o \
	arch/$(KLIBCARCH)/libgcc/__lshrdi3.o \
	arch/$(KLIBCARCH)/libgcc/__muldi3.o \
	arch/$(KLIBCARCH)/libgcc/__negdi2.o \
	libgcc/__divdi3.o \
	libgcc/__moddi3.o \
	libgcc/__udivdi3.o \
	libgcc/__umoddi3.o \
	libgcc/__udivmoddi4.o

archclean:
