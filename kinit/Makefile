SRCROOT = ..
include $(SRCROOT)/MCONFIG

MAKEDEPS := -Wp,-MD,.$(subst /,-,$*).d
CFLAGS	:= $(MAKEDEPS) $(OPTFLAGS) $(REQFLAGS) -W -Wall \
	-I$(SRCROOT)/ipconfig \
	-I$(SRCROOT)/nfsmount \
#	-DINI_DEBUG -g

LIBS	:= \
	$(SRCROOT)/nfsmount/libnfsmount.a \
	$(SRCROOT)/ipconfig/libipconfig.a \
	$(KLIBC) $(LIBGCC)
PROGS	:= kinit
OBJS	:= kinit.o do_mounts.o nfsroot.o

all: $(PROGS)

kinit: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $(OBJS) $(LIBS)

clean:
	-rm *.o kinit
