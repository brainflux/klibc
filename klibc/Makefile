CC	= gcc
LD      = ld
CFLAGS  = -I./include -I./linux/include -Wall -g -O2 -fomit-frame-pointer -march=i386 -mcpu=i686
LDFLAGS =
AR      = ar
RANLIB  = ranlib

TESTS   = 
LIBOBJS = vsnprintf.o snprintf.o vsscanf.o ctypes.o \
	  strntoumax.o strntoimax.o \
	  atoi.o atol.o atoll.o \
	  strtol.o strtoll.o strtoul.o strtoull.o \
	  strtoimax.o strtoumax.o \
	  _exit.o exit.o atexit.o onexit.o
LIB     = libmyc.a

SOFLAGS = -fPIC
SOOBJS  = $(patsubst %.o,%.lo,$(LIBOBJS))
SOLIB   = libmyc.so
SONAME  = libmyc.so

all: $(TESTS) crt0.o $(LIB) $(SOLIB)

.SUFFIXES: .c .o .a .so .lo .i .s .ls

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.c.i:
	$(CC) $(CFLAGS) -E -o $@ $<

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<

.c.lo:
	$(CC) $(CFLAGS) $(SOFLAGS) -c -o $@ $<

.c.ls:
	$(CC) $(CFLAGS) $(SOFLAGS) -S -o $@ $<

testvsnp: testvsnp.o $(LIB)
	$(CC) $(LDFLAGS) -o $@ testvsnp.o $(LIB)

$(LIB): $(LIBOBJS)
	rm -f $(LIB)
	$(AR) cq $(LIB) $(LIBOBJS)
	$(RANLIB) $(LIB)

$(SOLIB): $(SOOBJS)
	$(LD) -shared -soname $(SONAME) -o $@ $(SOOBJS) `$(CC) --print-libgcc`

clean:
	rm -f $(TESTS) *.o *.lo *.a *.so *.i *.s *.ls

vsnprintf.o: vsnprintf.c

vsnprintf.lo: vsnprintf.c

snprintf.o: snprintf.c

snprintf.lo: snprintf.c
