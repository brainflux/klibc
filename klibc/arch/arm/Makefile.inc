# -*- makefile -*-
#
# arch/arm/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

ifeq ($(THUMB),y)
ARCHOBJS = arch/arm/exits-thumb.o \
	   arch/arm/setjmp-thumb.o
LIBGCC   =
else
ARCHOBJS = arch/arm/exits-arm.o \
	   arch/arm/setjmp-arm.o
endif

ARCHSOOBJS = $(patsubst %.o,%.lo,$(ARCHOBJS))

archclean:
