KLIBSRC = ../klibc

include ../MCONFIG

CFLAGS  = $(OPTFLAGS) $(REQFLAGS) -W -Wall
LIBS	= $(KLIBC) $(LIBGCC)
PROGS   = chroot mkdir mkfifo mount pivot_root umount

all:	$(PROGS)

chroot:	chroot.o $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) chroot.o $(LIBS)

mkdir:	mkdir.o file_mode.o $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) mkdir.o file_mode.o $(LIBS)

mkfifo:	mkfifo.o file_mode.o $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) mkfifo.o file_mode.o $(LIBS)

mount:	mount.o mount_opts.o $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) mount.o mount_opts.o $(LIBS)

pivot_root: pivot_root.o $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) pivot_root.o $(LIBS)

umount: umount.o $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) umount.o $(LIBS)

$(CRT0) $(LIBS):
	@echo '*** error: $@ not up to date' || exit 1

clean:
	$(RM) *.o $(PROGS) core

spotless: clean
	$(RM) *~
