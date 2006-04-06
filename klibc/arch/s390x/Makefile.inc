# -*- makefile -*-
#
# arch/s390x/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

KLIBCARCHOBJS = \
	arch/$(KLIBCARCH)/setjmp.o \
	arch/$(KLIBCARCH)/mmap.o \
	arch/$(KLIBCARCH)/syscall.o

KLIBCARCHSOOBJS = $(patsubst %.o,%.lo,$(KLIBCARCHOBJS))


archclean:
