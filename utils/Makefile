KLIBSRC = ../klibc

include ../MCONFIG

CFLAGS  = $(OPTFLAGS) $(REQFLAGS)
LIBS	= $(KLIBC) $(LIBGCC)
PROGS   = mkdir mkfifo pivot_root

all:	$(PROGS)

mkdir:	mkdir.o file_mode.o $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) mkdir.o file_mode.o $(LIBS)

mkfifo:	mkfifo.o file_mode.o $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) mkfifo.o file_mode.o $(LIBS)

pivot_root: pivot_root.o $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) pivot_root.o $(LIBS)

clean:
	$(RM) *.o $(PROGS) core

spotless: clean
	$(RM) *~
