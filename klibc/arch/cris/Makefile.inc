# -*- makefile -*-
#
# arch/cris/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

ARCHOBJS = \
	arch/$(ARCH)/__UMod.o \
	arch/$(ARCH)/__UDiv.o \
	arch/$(ARCH)/__Mod.o \
	arch/$(ARCH)/__Div.o \
	arch/$(ARCH)/setjmp.o \
	arch/$(ARCH)/syscall.o \
	libgcc/__divdi3.o \
	libgcc/__moddi3.o \
	libgcc/__udivdi3.o \
	libgcc/__umoddi3.o \
	libgcc/__udivmoddi4.o \
	libgcc/__negdi2.o

arch/$(ARCH)/__UMod.o: arch/$(ARCH)/divide.c
	$(CC) $(CFLAGS) -DSIGNED=0 -DREM=1 -DBITS=32 -DNAME=__UMod -c -o $@ $<
arch/$(ARCH)/__UDiv.o: arch/$(ARCH)/divide.c
	$(CC) $(CFLAGS) -DSIGNED=0 -DREM=0 -DBITS=32 -DNAME=__UDiv -c -o $@ $<
arch/$(ARCH)/__Mod.o: arch/$(ARCH)/divide.c
	$(CC) $(CFLAGS) -DSIGNED=1 -DREM=1 -DBITS=32 -DNAME=__Mod -c -o $@ $<
arch/$(ARCH)/__Div.o: arch/$(ARCH)/divide.c
	$(CC) $(CFLAGS) -DSIGNED=1 -DREM=0 -DBITS=32 -DNAME=__Div -c -o $@ $<

archclean:
