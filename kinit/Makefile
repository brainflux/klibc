SRCROOT = ..
include $(SRCROOT)/MCONFIG

MAKEDEPS := -Wp,-MD,.$(subst /,-,$*).d
CFLAGS	:= $(MAKEDEPS) $(OPTFLAGS) $(REQFLAGS) -W -Wall \
	-I$(SRCROOT)/ipconfig \
	-I$(SRCROOT)/nfsmount
LIBS	:= \
	$(SRCROOT)/nfsmount/libnfsmount.a \
	$(SRCROOT)/ipconfig/libipconfig.a \
	$(KLIBC) $(LIBGCC)
PROGS	:= kinit
OBJS	:= kinit.o nfsroot.o

all: $(PROGS)

kinit: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $(OBJS) $(LIBS)

clean:
	-rm *.o
