# -*- makefile -*-
#
# arch/sparc64/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

OPTFLAGS = -Os -fomit-frame-pointer -m64 -mptr64
LDFLAGS  = -m elf64_sparc

archclean:
