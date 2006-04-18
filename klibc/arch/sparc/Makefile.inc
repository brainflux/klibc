# -*- makefile -*-
#
# arch/sparc/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

KLIBCARCHOBJS = \
	arch/$(KLIBCARCH)/sdiv.o \
	arch/$(KLIBCARCH)/udiv.o \
	arch/$(KLIBCARCH)/srem.o \
	arch/$(KLIBCARCH)/urem.o \
	arch/$(KLIBCARCH)/smul.o \
	arch/$(KLIBCARCH)/umul.o \
	arch/$(KLIBCARCH)/setjmp.o \
	arch/$(KLIBCARCH)/syscall.o \
	arch/$(KLIBCARCH)/sysfork.o \
	libgcc/__divdi3.o \
	libgcc/__moddi3.o \
	libgcc/__udivdi3.o \
	libgcc/__umoddi3.o \
	libgcc/__udivmoddi4.o

arch/$(KLIBCARCH)/sdiv.o: arch/$(KLIBCARCH)/sdiv.S

klibc/arch/$(KLIBCARCH)/sdiv.S: klibc/arch/$(KLIBCARCH)/divrem.m4
	@echo 'building $@ from $^'
	@(echo "define(NAME,\`.div')define(OP,\`div')define(S,\`true')"; \
	 cat $^) | m4 > $@
	@chmod 444 $@

arch/$(KLIBCARCH)/udiv.o: arch/$(KLIBCARCH)/udiv.S

klibc/arch/$(KLIBCARCH)/udiv.S: klibc/arch/$(KLIBCARCH)/divrem.m4
	@echo 'building $@ from $^'
	@(echo "define(NAME,\`.udiv')define(OP,\`div')define(S,\`false')"; \
	 cat $^) | m4 > $@
	@chmod 444 $@

arch/$(KLIBCARCH)/srem.o: arch/$(KLIBCARCH)/srem.S

klibc/arch/$(KLIBCARCH)/srem.S: klibc/arch/$(KLIBCARCH)/divrem.m4
	@echo 'building $@ from $^'
	@(echo "define(NAME,\`.rem')define(OP,\`rem')define(S,\`true')"; \
	 cat $^) | m4 > $@
	@chmod 444 $@

arch/$(KLIBCARCH)/urem.S: klibc/arch/$(KLIBCARCH)/urem.S

klibc/arch/$(KLIBCARCH)/urem.S: klibc/arch/$(KLIBCARCH)/divrem.m4
	@echo 'building $@ from $^'
	@(echo "define(NAME,\`.urem')define(OP,\`rem')define(S,\`false')"; \
	 cat $^) | m4 > $@
	@chmod 444 $@

archclean:
	rm -f arch/$(KLIBCARCH)/?div.S arch/$(KLIBCARCH)/?rem.S
