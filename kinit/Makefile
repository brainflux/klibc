SRCROOT = ..
include $(SRCROOT)/MCONFIG

MAKEDEPS = -Wp,-MD,.$(subst /,-,$*).d
CFLAGS   = $(MAKEDEPS) $(OPTFLAGS) $(REQFLAGS) -W -Wall \
	-I$(SRCROOT)/ipconfig \
	-I$(SRCROOT)/nfsmount
LIBS     = \
	$(SRCROOT)/nfsmount/libnfsmount.a \
	$(SRCROOT)/ipconfig/libipconfig.a \
	$(KLIBC) $(LIBGCC)
PROGS    = kinit

all: $(PROGS)

kinit: kinit.o
	$(LD) $(LDFLAGS) -o $@ $(CRT0) kinit.o $(LIBS)
