CC	= gcc
LD      = ld
CFLAGS  = -I. -I./include -I./linux/include -Wall -g -O2 -fomit-frame-pointer -march=i386 -mcpu=i686
LDFLAGS =
AR      = ar
RANLIB  = ranlib
PERL    = perl

TESTS   = 
LIBOBJS = vsnprintf.o snprintf.o vsprintf.o sprintf.o \
	  vsscanf.o ctypes.o \
	  strntoumax.o strntoimax.o \
	  atoi.o atol.o atoll.o \
	  strtol.o strtoll.o strtoul.o strtoull.o \
	  strtoimax.o strtoumax.o \
	  _exit.o exit.o atexit.o onexit.o \
	  wait.o wait3.o \
	  printf.o vprintf.o xwrite.o \
	  brk.o malloc.o free.o
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

$(LIB): $(LIBOBJS) syscalls.dir
	rm -f $(LIB)
	$(AR) cq $(LIB) $(LIBOBJS) syscalls/*.o
	$(RANLIB) $(LIB)

$(SOLIB): $(SOOBJS) syscalls.dir
	$(LD) -shared -soname $(SONAME) -o $@ \
		$(SOOBJS) syscalls/*.lo \
	        `$(CC) --print-libgcc`

syscalls.dir: SYSCALLS syscalls.pl syscommon.h
	rm -rf syscalls
	mkdir syscalls
	$(PERL) syscalls.pl < SYSCALLS
	$(MAKE) $(patsubst %.c,%.o,$(wildcard syscalls/*.c))
	$(MAKE) $(patsubst %.c,%.lo,$(wildcard syscalls/*.c))
	touch syscalls.dir

clean:
	rm -f $(TESTS) *.o *.lo *.a *.so *.i *.s *.ls
	rm -rf syscalls syscalls.dir

vsnprintf.o: vsnprintf.c

vsnprintf.lo: vsnprintf.c

snprintf.o: snprintf.c

snprintf.lo: snprintf.c
