include MCONFIG

COMMON_OBJS = main.o mount.o portmap.o dummypmap.o sunrpc.o
PROGS       = nfsmount
STATICPROGS := $(patsubst %,static/%,$(PROGS))
SHAREDPROGS := $(patsubst %,shared/%,$(PROGS))
LIB	    = libnfsmount.a
LIBS	    = $(KLIBC) $(LIBGCC)

OBJS := $(COMMON_OBJS) $(BOOTP_OBJS) $(DHCP_OBJS)

all:	$(STATICPROGS) $(SHAREDPROGS) $(LIB)

static/nfsmount: $(OBJS) $(CRT0) $(LIBS)
	mkdir -p static static.g
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $(OBJS) $(LIBS)
	cp -f $@ static.g
	$(STRIPCMD) $@

shared/nfsmount: $(OBJS) $(CRTSHARED) $(LIBSHARED) $(LIBGCC)
	mkdir -p shared shared.g
	$(LD) $(LDFLAGS) -o $@ $(EMAIN) $(CRTSHARED) $(OBJS) \
		-R $(LIBSHARED) $(LIBGCC)
	cp -f $@ shared.g
	$(STRIPCMD) $@

dummypmap: dummypmap_test.o
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $^ $(LIBS)

$(LIB): $(OBJS)
	$(AR) cru $(LIB) $(OBJS)

clean:
	$(RM) *.o $(LIB) core
	$(RM) -r static static.g shared shared.g

spotless: clean
	$(RM) *~ .*.d *~

ifneq ($(wildcard .*.d),)
include $(wildcard .*.d)
endif
