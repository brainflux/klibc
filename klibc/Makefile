ARCH    = alpha
CROSS   = 
CC	= $(CROSS)gcc
LD      = $(CROSS)ld
REQFLAGS = -nostdinc -iwithprefix include -I. \
	  -I./arch/$(ARCH)/include -I./include/bits$(BITSIZE) \
	  -I./include -I./linux/include \
	  -Wall
CFLAGS  = $(OPTFLAGS) $(REQFLAGS)
LDFLAGS =
AR      = $(CROSS)ar
RANLIB  = $(CROSS)ranlib
PERL    = perl
STRIP   = $(CROSS)strip -R .comment -R .note

TESTS   = testvsnp hello minihello microhello getenvtest \
	  getopttest malloctest minips idtest
LIBOBJS = vsnprintf.o snprintf.o vsprintf.o sprintf.o \
	  vsscanf.o sscanf.o ctypes.o \
	  strntoumax.o strntoimax.o \
	  atoi.o atol.o atoll.o \
	  strtol.o strtoll.o strtoul.o strtoull.o \
	  strtoimax.o strtoumax.o \
	  __main.o globals.o exitc.o exits.o atexit.o onexit.o \
	  execl.o execle.o execv.o execvpe.o execvp.o execlp.o execlpe.o \
	  fork.o wait.o wait3.o waitpid.o setpgrp.o \
	  printf.o vprintf.o fprintf.o vfprintf.o \
	  fread.o fread2.o fwrite.o fwrite2.o fputs.o puts.o \
	  sleep.o usleep.o raise.o abort.o assert.o alarm.o pause.o \
	  signal.o sigaction.o sigpending.o sigprocmask.o sigsuspend.o \
	  brk.o sbrk.o malloc.o realloc.o calloc.o mmap.o \
	  memcpy.o memset.o memccpy.o memmem.o strcat.o strchr.o \
	  strcmp.o strcpy.o strdup.o strlen.o strncat.o strstr.o \
	  strncmp.o strncpy.o strrchr.o strspn.o strsep.o strtok.o \
	  gethostname.o getdomainname.o getcwd.o seteuid.o setegid.o \
	  getenv.o setenv.o unsetenv.o getopt.o readdir.o \
	  time.o fdatasync.o
LIB     = libc.a

SOFLAGS = -fPIC
SOOBJS  = $(patsubst %.o,%.lo,$(LIBOBJS))
SOLIB   = libc.so
SONAME  = libc.so
CRT0    = crt0.o
LIBGCC  = $(shell $(CC) --print-libgcc)

all: $(TESTS) $(CRT0) $(LIB) $(SOLIB)

#
# Include arch-specific rule fragments
#
include arch/$(ARCH)/Makefile.inc

.SUFFIXES: .c .o .a .so .lo .i .S .s .ls .ss .lss

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

.s.o:
	$(CC) $(CFLAGS) -x assembler -c -o $@ $<

.ls.lo:
	$(CC) $(CFLAGS) $(SOFLAGS) -x assembler -c -o $@ $<

.c.lo:
	$(CC) $(CFLAGS) $(SOFLAGS) -c -o $@ $<

.c.ls:
	$(CC) $(CFLAGS) $(SOFLAGS) -S -o $@ $<

% : %.o $(LIB) $(CRT0)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $< $(LIB) $(LIBGCC)
	cp $@ $@.stripped
	$(STRIP) $@.stripped

testvsnp: testvsnp.o $(LIB) $(CRT0)

hello: hello.o $(LIB) $(CRT0)

minihello: minihello.o $(LIB) $(CRT0)

microhello: microhello.o $(LIB) $(CRT0)

getenvtest: getenvtest.o $(LIB) $(CRT0)

getopttest: getopttest.o $(LIB) $(CRT0)

malloctest: malloctest.o $(LIB) $(CRT0)

minips: minips.o $(LIB) $(CRT0)

idtest: idtest.o $(LIB) $(CRT0)

$(LIB): $(LIBOBJS) $(ARCHOBJS) syscalls/static.obj socketcalls/static.obj
	rm -f $(LIB)
	$(AR) cq $(LIB) $(LIBOBJS) $(ARCHOBJS) syscalls/*.o socketcalls/*.o
	$(RANLIB) $(LIB)

$(SOLIB): $(SOOBJS) $(ARCHSOOBJS) syscalls/shared.obj socketcalls/shared.obj
	$(LD) -shared -soname $(SONAME) -o $@ \
		$(SOOBJS) $(ARCHSOOBJS) syscalls/*.lo socketcalls/*.lo \
	        $(LIBGCC)

crt0.o: arch/$(ARCH)/crt0.o
	cp arch/$(ARCH)/crt0.o .

syscalls.dir: SYSCALLS syscalls.pl syscommon.h
	rm -rf syscalls
	mkdir syscalls
	$(PERL) syscalls.pl $(ARCH) < SYSCALLS
	touch $@

socketcalls.dir: SOCKETCALLS socketcalls.pl socketcommon.h
	rm -rf socketcalls
	mkdir socketcalls
	$(PERL) socketcalls.pl $(ARCH) < SOCKETCALLS
	touch $@

%/static.obj: %.dir
	$(MAKE) objects-$(notdir $(@:.obj=)) DIR=$*

%/shared.obj: %.dir
	$(MAKE) objects-$(notdir $(@:.obj=)) DIR=$*

STATIC = $(addsuffix .o,$(basename $(wildcard $(DIR)/*.[cS])))
SHARED = $(addsuffix .lo,$(basename $(wildcard $(DIR)/*.[cS])))

objects-static: $(STATIC)
	touch $(DIR)/static.obj

objects-shared: $(SHARED)
	touch $(DIR)/shared.obj

clean: archclean
	rm -f $(TESTS) *.o *.lo *.a *.so *.i *.s *.ls *.stripped
	rm -f arch/*/*.o arch/*/*.lo arch/*/*.i arch/*/*.s arch/*/*.ls
	rm -rf syscalls syscalls.dir
	rm -rf socketcalls socketcalls.dir

spotless: clean
	find . -name \*~ -not -type d -print0 | xargs -0rt rm -f
