include MCONFIG

COMMON_OBJS = main.o netdev.o packet.o
DHCP_OBJS   = dhcp_proto.o
BOOTP_OBJS  = bootp_proto.o
PROG        = ipconfig
LIB	    = libipconfig.a
LIBS        = $(KLIBC) $(LIBGCC)

OBJS := $(COMMON_OBJS) $(BOOTP_OBJS) $(DHCP_OBJS)

all:	$(PROG) $(LIB)

ipconfig: $(OBJS) $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $(OBJS) $(LIBS)

$(LIB): $(OBJS)
	$(AR) cru $(LIB) $(OBJS)

clean:
	$(RM) *.o $(PROGS) core

spotless:
	$(RM) *~ .*.d

ifneq ($(wildcard .*.d),)
include $(wildcard .*.d)
endif
