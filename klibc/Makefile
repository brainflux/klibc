#
# Makefile
#
# Main makefile
#

# Include configuration rules
include MCONFIG

TESTS   = $(patsubst %.c,%,$(wildcard tests/*.c))
LIBOBJS = vsnprintf.o snprintf.o vsprintf.o sprintf.o \
	  vsscanf.o sscanf.o ctypes.o \
	  strntoumax.o strntoimax.o \
	  atoi.o atol.o atoll.o \
	  strtol.o strtoll.o strtoul.o strtoull.o \
	  strtoimax.o strtoumax.o \
	  __main.o globals.o exitc.o atexit.o onexit.o \
	  execl.o execle.o execv.o execvpe.o execvp.o execlp.o execlpe.o \
	  fork.o wait.o wait3.o waitpid.o setpgrp.o \
	  printf.o vprintf.o fprintf.o vfprintf.o \
	  fopen.o fread.o fread2.o fwrite.o fwrite2.o fputs.o puts.o \
	  sleep.o usleep.o raise.o abort.o assert.o alarm.o pause.o \
	  signal.o siglist.o \
	  sigaction.o sigpending.o sigprocmask.o sigsuspend.o \
	  brk.o sbrk.o malloc.o realloc.o calloc.o mmap.o \
	  memcpy.o memcmp.o memset.o memccpy.o memmem.o memswap.o \
	  strcat.o strchr.o strcmp.o strcpy.o strdup.o strlen.o \
	  strncat.o strstr.o strncmp.o strncpy.o strrchr.o strspn.o \
	  strsep.o strtok.o \
	  gethostname.o getdomainname.o getcwd.o seteuid.o setegid.o \
	  getenv.o setenv.o unsetenv.o getopt.o readdir.o \
	  syslog.o closelog.o pty.o isatty.o \
	  time.o fdatasync.o llseek.o select.o nice.o getpriority.o \
	  qsort.o lrand48.o srand48.o seed48.o \
	  inet/inet_ntoa.o inet/inet_aton.o inet/inet_addr.o \
	  inet/inet_ntop.o inet/inet_pton.o
LIB     = libc.a

all: tests $(CRT0) $(LIB) $(SOLIB)

# Add any architecture-specific rules
include arch/$(ARCH)/Makefile.inc

tests: $(TESTS)

tests/% : tests/%.c $(LIB) $(CRT0)
	$(CC) $(CFLAGS) -c -o tests/$*.o $<
	$(LD) $(LDFLAGS) -o $@ $(CRT0) tests/$*.o $(LIB) $(LIBGCC)
	cp $@ $@.stripped
	$(STRIP) $@.stripped

$(LIB): $(LIBOBJS) $(ARCHOBJS) syscalls/static.obj socketcalls/static.obj
	rm -f $(LIB)
	$(AR) cq $(LIB) $(LIBOBJS) $(ARCHOBJS) syscalls/*.o socketcalls/*.o
	$(RANLIB) $(LIB)

$(SOLIB): $(SOOBJS) $(ARCHSOOBJS) syscalls/shared.obj socketcalls/shared.obj
	$(LD) $(LDFLAGS) -shared -soname $(SONAME) -o $@ \
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
	find . -type f -a \( -name \*.[isoa] -o -name \*.l[iso] \) -print0 | xargs -0rt rm -f
	rm -f *.a *.so
	rm -f $(TESTS) tests/*.stripped
	rm -rf syscalls syscalls.dir
	rm -rf socketcalls socketcalls.dir

spotless: clean
	find . -name \*~ -not -type d -print0 | xargs -0rt rm -f
