CC	= gcc
LD      = ld
CFLAGS  = -I. -I./include -I./linux/include -Wall -g -O2 -fomit-frame-pointer -march=i386 -mcpu=i686
LDFLAGS =
AR      = ar
RANLIB  = ranlib
PERL    = perl

TESTS   = testvsnp hello minihello
LIBOBJS = vsnprintf.o snprintf.o vsprintf.o sprintf.o \
	  vsscanf.o ctypes.o \
	  strntoumax.o strntoimax.o \
	  atoi.o atol.o atoll.o \
	  strtol.o strtoll.o strtoul.o strtoull.o \
	  strtoimax.o strtoumax.o \
	  _exit.o exit.o atexit.o onexit.o \
	  execl.o execv.o wait.o wait3.o setpgrp.o \
	  printf.o vprintf.o xread.o xwrite.o fputs.o \
	  sleep.o usleep.o \
	  brk.o malloc.o free.o realloc.o calloc.o \
	  raise.o abort.o assert.o \
	  memcpy.o memset.o memccpy.o \
	  strcat.o strchr.o strcmp.o strcpy.o strdup.o strlen.o strncat.o \
	  strncmp.o strncpy.o strrchr.o strspn.o strsep.o strtok.o \
	  gethostname.o getdomainname.o getcwd.o seteuid.o setegid.o
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

minihello: minihello.o $(LIB) $(CRT0)
	$(LD) -o minihello $(CRT0) minihello.o $(LIB) $(LIBGCC)

$(LIB): $(LIBOBJS) syscalls.dir socketcalls.dir
	rm -f $(LIB)
	$(AR) cq $(LIB) $(LIBOBJS) syscalls/*.o socketcalls/*.o
	$(RANLIB) $(LIB)

$(SOLIB): $(SOOBJS) syscalls.dir
	$(LD) -shared -soname $(SONAME) -o $@ \
		$(SOOBJS) syscalls/*.lo socketcalls/*.lo \
	        $(LIBGCC)

syscalls.dir: SYSCALLS syscalls.pl syscommon.h
	rm -rf syscalls
	mkdir syscalls
	$(PERL) syscalls.pl < SYSCALLS
	$(MAKE) syscalls-objs
	touch syscalls.dir

SYSCALLS = $(patsubst %.c,%.o,$(wildcard syscalls/*.c)) \
	   $(patsubst %.c,%.lo,$(wildcard syscalls/*.c))
syscalls-objs: $(SYSCALLS)

socketcalls.dir: SOCKETCALLS socketcalls.pl socketcommon.h
	rm -rf socketcalls
	mkdir socketcalls
	$(PERL) socketcalls.pl < SOCKETCALLS
	$(MAKE) socketcalls-objs
	touch socketcalls.dir

SOCKETCALLS = $(patsubst %.c,%.o,$(wildcard socketcalls/*.c)) \
	      $(patsubst %.c,%.lo,$(wildcard socketcalls/*.c))
socketcalls-objs: $(SOCKETCALLS)

clean:
	rm -f $(TESTS) *.o *.lo *.a *.so *.i *.s *.ls
	rm -rf syscalls syscalls.dir
	rm -rf socketcalls socketcalls.dir
