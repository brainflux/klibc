# -*- makefile -*-
#
# arch/cris/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

KLIBCARCHOBJS = \
	arch/$(KLIBCARCH)/__Umod.o \
	arch/$(KLIBCARCH)/__Udiv.o \
	arch/$(KLIBCARCH)/__Mod.o \
	arch/$(KLIBCARCH)/__Div.o \
	arch/$(KLIBCARCH)/__negdi2.o \
	arch/$(KLIBCARCH)/setjmp.o \
	arch/$(KLIBCARCH)/syscall.o \
	libgcc/__divdi3.o \
	libgcc/__moddi3.o \
	libgcc/__udivdi3.o \
	libgcc/__umoddi3.o \
	libgcc/__udivmoddi4.o

arch/$(KLIBCARCH)/__Umod.o: arch/$(KLIBCARCH)/divide.c
	$(CC) $(CFLAGS) -DSIGNED=0 -DREM=1 -DBITS=32 -DNAME=__Umod -c -o $@ $<
arch/$(KLIBCARCH)/__Udiv.o: arch/$(KLIBCARCH)/divide.c
	$(CC) $(CFLAGS) -DSIGNED=0 -DREM=0 -DBITS=32 -DNAME=__Udiv -c -o $@ $<
arch/$(KLIBCARCH)/__Mod.o: arch/$(KLIBCARCH)/divide.c
	$(CC) $(CFLAGS) -DSIGNED=1 -DREM=1 -DBITS=32 -DNAME=__Mod -c -o $@ $<
arch/$(KLIBCARCH)/__Div.o: arch/$(KLIBCARCH)/divide.c
	$(CC) $(CFLAGS) -DSIGNED=1 -DREM=0 -DBITS=32 -DNAME=__Div -c -o $@ $<

archclean:
