# -*- makefile -*-
#
# arch/mips/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

ARCHOBJS = \
	arch/$(ARCH)/pipe.o \
	arch/$(ARCH)/vfork.o

ARCHSOOBJS = $(patsubst %.o,%.lo,$(ARCHOBJS))


archclean:
