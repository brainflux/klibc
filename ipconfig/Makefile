include MCONFIG

COMMON_OBJS = main.o netdev.o packet.o
DHCP_OBJS   = dhcp_proto.o
BOOTP_OBJS  = bootp_proto.o
PROGS       = ipconfig
STATICPROGS := $(patsubst %,static/%,$(PROGS))
SHAREDPROGS := $(patsubst %,shared/%,$(PROGS))
LIB	    = libipconfig.a
LIBS        = $(KLIBC) $(LIBGCC)

OBJS := $(COMMON_OBJS) $(BOOTP_OBJS) $(DHCP_OBJS)

all:	$(STATICPROGS) $(SHAREDPROGS) $(LIB)

static/ipconfig: $(OBJS) $(CRT0) $(LIBS)
	mkdir -p static static.g
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $(OBJS) $(LIBS)
	cp -f $@ static.g
	$(STRIPCMD) $@

shared/ipconfig: $(OBJS) $(CRTSHARED) $(LIBSHARED) $(LIBGCC)
	mkdir -p shared shared.g
	$(LD) $(LDFLAGS) -o $@ $(EMAIN) $(CRTSHARED) $(OBJS) \
		-R $(LIBSHARED) $(LIBGCC)
	cp -f $@ shared.g
	$(STRIPCMD) $@

$(LIB): $(OBJS)
	$(AR) cru $(LIB) $(OBJS)

clean:
	$(RM) *.o $(PROG) $(LIB) core
	$(RM) -r static static.g shared shared.g

spotless: clean
	$(RM) *~ .*.d

ifneq ($(wildcard .*.d),)
include $(wildcard .*.d)
endif
