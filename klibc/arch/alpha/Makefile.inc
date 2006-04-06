# -*- makefile -*-
#
# arch/alpha/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

# Special CFLAGS for the divide code
DIVCFLAGS = $(KLIBCREQFLAGS) $(KLIBCKLIBCARCHREQFLAGS) \
	-O3 -fomit-frame-pointer -fcall-saved-1 -fcall-saved-2 \
	-fcall-saved-3 -fcall-saved-4 -fcall-saved-5 -fcall-saved-6 \
	-fcall-saved-7 -fcall-saved-8 -ffixed-15 -fcall-saved-16 \
	-fcall-saved-17 -fcall-saved-18 -fcall-saved-19 -fcall-saved-20 \
	-fcall-saved-21 -fcall-saved-22 -ffixed-23 -fcall-saved-24 \
	-ffixed-25 -ffixed-27

KLIBCARCHOBJS := arch/$(KLIBCARCH)/pipe.o     arch/$(KLIBCARCH)/setjmp.o
KLIBCARCHOBJS += arch/$(KLIBCARCH)/syscall.o  arch/$(KLIBCARCH)/sysdual.o

DIVOBJS += arch/$(KLIBCARCH)/__divqu.o \
           arch/$(KLIBCARCH)/__remqu.o \
           arch/$(KLIBCARCH)/__divq.o \
           arch/$(KLIBCARCH)/__remq.o \
           arch/$(KLIBCARCH)/__divlu.o \
           arch/$(KLIBCARCH)/__remlu.o \
           arch/$(KLIBCARCH)/__divl.o \
           arch/$(KLIBCARCH)/__reml.o

KLIBCARCHOBJS += $(DIVOBJS)

quiet_cmd_regswap = REGSWAP $@
      cmd_regswap = sed -e 's/\$$0\b/$$27/g'  -e 's/\$$24\b/$$99/g' \
                        -e 's/\$$16\b/$$24/g' -e 's/\$$17\b/$$25/g' \
                        -e 's/\$$26\b/$$23/g' -e 's/\$$99\b/$$16/g' < $< > $@

# Use static pattern rule to avoid using a temporary file
$(addprefix $(obj)/,$(DIVOBJS:.o=.S)): $(obj)/arch/$(KLIBCARCH)/%.S: \
                                       $(obj)/arch/$(KLIBCARCH)/%.ss
	$(call if_changed,regswap)

quiet_cmd_genss = DIV-CC  $@
      cmd_genss = $(CC) $(DIVCFLAGS) $(FILE_CFLAGS) \
                        -DNAME=$(basename $(notdir $@)) -S -o $@ $<

$(obj)/arch/$(KLIBCARCH)/%.ss: $(obj)/arch/$(KLIBCARCH)/divide.c
	$(call if_changed,genss)

$(obj)/arch/$(KLIBCARCH)/__divqu.ss: FILE_CFLAGS := -DSIGNED=0 -DREM=0 -DBITS=64
$(obj)/arch/$(KLIBCARCH)/__remqu.ss: FILE_CFLAGS := -DSIGNED=0 -DREM=1 -DBITS=64
$(obj)/arch/$(KLIBCARCH)/__divq.ss:  FILE_CFLAGS := -DSIGNED=1 -DREM=0 -DBITS=64
$(obj)/arch/$(KLIBCARCH)/__remq.ss:  FILE_CFLAGS := -DSIGNED=1 -DREM=1 -DBITS=64
$(obj)/arch/$(KLIBCARCH)/__divlu.ss: FILE_CFLAGS := -DSIGNED=0 -DREM=0 -DBITS=32
$(obj)/arch/$(KLIBCARCH)/__remlu.ss: FILE_CFLAGS := -DSIGNED=0 -DREM=1 -DBITS=32
$(obj)/arch/$(KLIBCARCH)/__divl.ss:  FILE_CFLAGS := -DSIGNED=1 -DREM=0 -DBITS=32
$(obj)/arch/$(KLIBCARCH)/__reml.ss:  FILE_CFLAGS := -DSIGNED=1 -DREM=1 -DBITS=32

targets     += $(DIVOBJS:.o=.S) $(DIVOBJS:.o=.ss)
clean-files += $(DIVOBJS:.o=.S) $(DIVOBJS:.o=.ss)
