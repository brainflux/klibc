CC	= gcc
LD      = ld
CFLAGS  = -I./include -Wall -g -O2 -fomit-frame-pointer -march=i386 -mcpu=i686
LDFLAGS =
AR      = ar
RANLIB  = ranlib

TESTS   = testvsnp
LIBOBJS = vsnprintf.o snprintf.o vsscanf.o ctypes.o \
	  strntoumax.o atoi.o atol.o atoll.o \
	  strtol.o strtoll.o strtoul.o strtoull.o \
	  strtoimax.o strtoumax.o
LIB     = libmyc.a

SOFLAGS = -fPIC
SOOBJS  = $(patsubst %.o,%.lo,$(LIBOBJS))
SOLIB   = libmyc.so
SONAME  = libmyc.so

all: $(TESTS) $(LIB) $(SOLIB)

.SUFFIXES: .c .o .a .so .lo

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.c.lo:
	$(CC) $(CFLAGS) $(SOFLAGS) -c -o $@ $<

testvsnp: testvsnp.o $(LIB)
	$(CC) $(LDFLAGS) -o $@ testvsnp.o $(LIB)

$(LIB): $(LIBOBJS)
	rm -f $(LIB)
	$(AR) cq $(LIB) $(LIBOBJS)
	$(RANLIB) $(LIB)

$(SOLIB): $(SOOBJS)
	$(LD) -shared -soname $(SONAME) -o $@ $(SOOBJS) `$(CC) --print-libgcc`

clean:
	rm -f $(TESTS) *.o *.lo *.a *.so

vsnprintf.o: vsnprintf.c

vsnprintf.lo: vsnprintf.c

snprintf.o: snprintf.c

snprintf.lo: snprintf.c
