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
	arch/$(ARCH)/socketcall.o

ARCHSOOBJS = $(patsubst %.o,%.lo,$(ARCHOBJS))

archclean:
