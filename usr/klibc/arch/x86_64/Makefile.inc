# -*- makefile -*-
#
# arch/x86_64/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

KLIBCARCHOBJS = \
	arch/$(KLIBCARCH)/setjmp.o \
	arch/$(KLIBCARCH)/syscall.o \
	arch/$(KLIBCARCH)/sigreturn.o \
	arch/$(KLIBCARCH)/vfork.o

KLIBCARCHSOOBJS = $(patsubst %.o,%.lo,$(KLIBCARCHOBJS))

archclean:
