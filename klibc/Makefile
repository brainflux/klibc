ARCH    = i386
CC	= gcc
LD      = ld
OPTFLAGS = -Os -fomit-frame-pointer -march=i386 -mcpu=i686 \
	   -malign-functions=0 -malign-jumps=0 -malign-loops=0
CFLAGS  = -I. -I./include -I./linux/include -Wall -g $(OPTFLAGS)
LDFLAGS =
AR      = ar
RANLIB  = ranlib
PERL    = perl

TESTS   = testvsnp hello minihello microhello getenvtest \
	  getopttest malloctest
LIBOBJS = vsnprintf.o snprintf.o vsprintf.o sprintf.o \
	  vsscanf.o ctypes.o \
	  strntoumax.o strntoimax.o \
	  atoi.o atol.o atoll.o \
	  strtol.o strtoll.o strtoul.o strtoull.o \
	  strtoimax.o strtoumax.o \
	  exitc.o exits.o atexit.o onexit.o \
	  execl.o execle.o execv.o \
	  wait.o wait3.o waitpid.o setpgrp.o \
	  printf.o vprintf.o xread.o xwrite.o fputs.o \
	  sleep.o usleep.o raise.o abort.o assert.o \
	  signal.o sigaction.o sigpending.o sigprocmask.o sigsuspend.o \
	  brk.o sbrk.o malloc.o free.o realloc.o calloc.o mmap.o \
	  memcpy.o memset.o memccpy.o memmem.o strcat.o strchr.o \
	  strcmp.o strcpy.o strdup.o strlen.o strncat.o strstr.o \
	  strncmp.o strncpy.o strrchr.o strspn.o strsep.o strtok.o \
	  gethostname.o getdomainname.o getcwd.o seteuid.o setegid.o \
	  getenv.o setenv.o unsetenv.o getopt.o
LIB     = libc.a

SOFLAGS = -fPIC
SOOBJS  = $(patsubst %.o,%.lo,$(LIBOBJS))
SOLIB   = libc.so
SONAME  = libc.so
CRT0    = crt0.o
LIBGCC  = $(shell $(CC) --print-libgcc)
STRIP   = strip -R .comment -R .note

all: $(TESTS) $(CRT0) $(LIB) $(SOLIB)

.SUFFIXES: .c .o .a .so .lo .i .S .s .ls

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.c.i:
	$(CC) $(CFLAGS) -E -o $@ $<

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<

.S.o:
	$(CC) $(CFLAGS) -D__ASSEMBLY__ -c -o $@ $<

.S.lo:
	$(CC) $(CFLAGS) $(SOFLAGS) -D__ASSEMBLY__ -c -o $@ $<

.c.lo:
	$(CC) $(CFLAGS) $(SOFLAGS) -c -o $@ $<

.c.ls:
	$(CC) $(CFLAGS) $(SOFLAGS) -S -o $@ $<

# Kill implicit rule
.o:

% : %.o $(LIB) $(CRT0)
	$(LD) -o $@ $(CRT0) $< $(LIB) $(LIBGCC)
	cp $@ $@.stripped
	$(STRIP) $@.stripped

testvsnp: testvsnp.o $(LIB) $(CRT0)

hello: hello.o $(LIB) $(CRT0)

minihello: minihello.o $(LIB) $(CRT0)

microhello: microhello.o $(LIB) $(CRT0)

getenvtest: getenvtest.o $(LIB) $(CRT0)

getopttest: getopttest.o $(LIB) $(CRT0)

malloctest: malloctest.o $(LIB) $(CRT0)

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
	$(PERL) syscalls.pl $(ARCH) < SYSCALLS
	$(MAKE) syscalls-objs
	touch syscalls.dir

SYSCALLS = \
	$(addsuffix .o,$(basename $(wildcard syscalls/*.[cS]))) \
	$(addsuffix .lo,$(basename $(wildcard syscalls/*.[cS])))
syscalls-objs: $(SYSCALLS)

socketcalls.dir: SOCKETCALLS socketcalls.pl socketcommon.h
	rm -rf socketcalls
	mkdir socketcalls
	$(PERL) socketcalls.pl $(ARCH) < SOCKETCALLS
	$(MAKE) socketcalls-objs
	touch socketcalls.dir

SOCKETCALLS = \
	$(addsuffix .o,$(basename $(wildcard socketcalls/*.[cS]))) \
	$(addsuffix .lo,$(basename $(wildcard socketcalls/*.[cS])))
socketcalls-objs: $(SOCKETCALLS)

clean:
	rm -f $(TESTS) *.o *.lo *.a *.so *.i *.s *.ls *.stripped
	rm -rf syscalls syscalls.dir
	rm -rf socketcalls socketcalls.dir
