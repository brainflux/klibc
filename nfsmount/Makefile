include MCONFIG

COMMON_OBJS = main.o mount.o portmap.o sunrpc.o
PROG        = nfsmount
LIB	    = libnfsmount.a
LIBS        = $(KLIBC) $(LIBGCC)

OBJS := $(COMMON_OBJS) $(BOOTP_OBJS) $(DHCP_OBJS)

all:	$(PROG) $(LIB)

nfsmount: $(OBJS) $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $(OBJS) $(LIBS)

$(LIB): $(OBJS)
	$(AR) cru $(LIB) $(OBJS)

clean:
	$(RM) *.o $(PROG) $(LIB) core

spotless: clean
	$(RM) *~ .*.d *~

ifneq ($(wildcard .*.d),)
include $(wildcard .*.d)
endif
