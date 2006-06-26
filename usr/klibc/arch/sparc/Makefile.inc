# -*- makefile -*-
#
# arch/sparc/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

m4-targets := arch/$(KLIBCARCH)/sdiv.o arch/$(KLIBCARCH)/srem.o \
              arch/$(KLIBCARCH)/udiv.o arch/$(KLIBCARCH)/urem.o

KLIBCARCHOBJS = $(m4-targets) \
	arch/$(KLIBCARCH)/smul.o \
	arch/$(KLIBCARCH)/umul.o \
	arch/$(KLIBCARCH)/__muldi3.o \
	arch/$(KLIBCARCH)/setjmp.o \
	arch/$(KLIBCARCH)/pipe.o \
	arch/$(KLIBCARCH)/syscall.o \
	arch/$(KLIBCARCH)/sysfork.o \
	libgcc/__ashldi3.o \
	libgcc/__ashrdi3.o \
	libgcc/__lshrdi3.o \
	libgcc/__divdi3.o \
	libgcc/__moddi3.o \
	libgcc/__udivdi3.o \
	libgcc/__umoddi3.o \
	libgcc/__udivmoddi4.o \
	libgcc/__clzsi2.o \


adir := $(obj)/arch/$(KLIBCARCH)

$(adir)/sdiv.S: m4 := define(NAME,\`.div')define(OP,\`div')define(S,\`true')
$(adir)/srem.S: m4 := define(NAME,\`.rem')define(OP,\`rem')define(S,\`true')
$(adir)/udiv.S: m4 := define(NAME,\`.udiv')define(OP,\`div')define(S,\`false')
$(adir)/urem.S: m4 := define(NAME,\`.urem')define(OP,\`rem')define(S,\`false')

targets += $(m4-targets) $(m4-targets:.o=.S)

quiet_cmd_m4 = M4      $@
      cmd_m4 = (echo "$(m4)"; cat $^) | m4 > $@

# build .o from .S
$(addprefix $(obj)/,$(m4-targets)): $(adir)/%.o : $(adir)/%.S
# build .S from .m4
$(addprefix $(obj)/,$(m4-targets:.o=.S)): $(src)/arch/$(KLIBCARCH)/divrem.m4
	$(call if_changed,m4)
