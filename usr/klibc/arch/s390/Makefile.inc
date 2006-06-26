# -*- makefile -*-
#
# arch/s390/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

ifneq ("$(KLIBCARCH)", "s390x")

KLIBCARCHOBJS = \
	arch/$(KLIBCARCHDIR)/setjmp.o \
	arch/$(KLIBCARCHDIR)/mmap.o \
	arch/$(KLIBCARCHDIR)/syscall.o \
	libgcc/__clzsi2.o \
	libgcc/__ashldi3.o \
	libgcc/__ashrdi3.o \
	libgcc/__lshrdi3.o \
	libgcc/__divdi3.o \
	libgcc/__moddi3.o \
	libgcc/__udivdi3.o \
	libgcc/__umoddi3.o \
	libgcc/__udivmoddi4.o

else

KLIBCARCHOBJS = \
	arch/$(KLIBCARCHDIR)/setjmp.o \
	arch/$(KLIBCARCHDIR)/mmap.o \
	arch/$(KLIBCARCHDIR)/syscall.o

endif

KLIBCARCHSOOBJS = $(patsubst %.o,%.lo,$(KLIBCARCHOBJS))


archclean:
