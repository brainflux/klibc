CC	= gcc
LD      = ld
CFLAGS  = -I. -I./include -I./linux/include -Wall -g -O2 -fomit-frame-pointer -march=i386 -mcpu=i686
LDFLAGS =
AR      = ar
RANLIB  = ranlib
PERL    = perl

TESTS   = testvsnp hello
LIBOBJS = vsnprintf.o snprintf.o vsprintf.o sprintf.o \
	  vsscanf.o ctypes.o \
	  strntoumax.o strntoimax.o \
	  atoi.o atol.o atoll.o \
	  strtol.o strtoll.o strtoul.o strtoull.o \
	  strtoimax.o strtoumax.o \
	  _exit.o exit.o atexit.o onexit.o \
	  wait.o wait3.o \
	  printf.o vprintf.o xwrite.o fputs.o \
	  brk.o malloc.o free.o \
	  raise.o abort.o assert.o \
	  memcpy.o memset.o memccpy.o \
	  strlen.o strcpy.o strncpy.o strchr.o strrchr.o
LIB     = libc.a

SOFLAGS = -fPIC
SOOBJS  = $(patsubst %.o,%.lo,$(LIBOBJS))
SOLIB   = libc.so
SONAME  = libc.so
CRT0    = crt0.o
LIBGCC  = $(shell $(CC) --print-libgcc)

all: $(TESTS) $(CRT0) $(LIB) $(SOLIB)

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

testvsnp: testvsnp.o $(LIB) $(CRT0)
	$(LD) -o testvsnp $(CRT0) testvsnp.o $(LIB) $(LIBGCC)

hello: hello.o $(LIB) $(CRT0)
	$(LD) -o hello $(CRT0) hello.o $(LIB) $(LIBGCC)

$(LIB): $(LIBOBJS) syscalls.dir
	rm -f $(LIB)
	$(AR) cq $(LIB) $(LIBOBJS) syscalls/*.o
	$(RANLIB) $(LIB)

$(SOLIB): $(SOOBJS) syscalls.dir
	$(LD) -shared -soname $(SONAME) -o $@ \
		$(SOOBJS) syscalls/*.lo \
	        $(LIBGCC)

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
