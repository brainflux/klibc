SRCROOT  = ..
include ../MCONFIG
include ../MRULES

MAKEDEPS     = -Wp,-MD,.$(subst /,-,$*).d
CFLAGS       = $(MAKEDEPS) $(OPTFLAGS) $(REQFLAGS) -W -Wall
LIBS         = $(KLIBC) $(LIBGCC)
PROGS       := chroot dd fstype mkdir mkfifo mount pivot_root umount \
	       true false sleep
STATICPROGS := $(patsubst %,static/%,$(PROGS))
SHAREDPROGS := $(patsubst %,shared/%,$(PROGS))
LIBOBJS	     = file_mode.o
LIBUTILS     = libutils.a

all:	$(STATICPROGS) $(SHAREDPROGS)

static/%: %.o $(CRT0) $(LIBS) $(LIBUTILS)
	mkdir -p static
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $< $(LIBUTILS) $(LIBS)
	$(STRIP) $@

shared/%: %.o $(CRTSHARED) $(LIBSHARED) $(LIBUTILS)
	mkdir -p shared
	$(LD) $(LDFLAGS) -o $@ -e main $(CRTSHARED) $< $(LIBUTILS) \
		-R $(LIBSHARED) $(LIBGCC)
	$(STRIP) $@

# Programs that consist of more than one file
mount.o: mount_main.o mount_opts.o
	$(LD) -r -o $@ $^

$(LIBUTILS): $(LIBOBJS)
	-rm -f $@
	$(AR) cq $@ $^
	$(RANLIB) $@

$(CRT0) $(LIBS):
	@echo '*** error: $@ not up to date' || exit 1

clean:
	$(RM) *.o core $(PROGS) .*.d shared/* static/*

spotless: clean
	$(RM) *~

ifneq ($(wildcard .*.d),)
include $(wildcard .*.d)
endif
