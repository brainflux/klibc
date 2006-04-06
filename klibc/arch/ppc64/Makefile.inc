# -*- makefile -*-
#
# arch/ppc64/Makefile.inc
#
# Special rules for this architecture.  Note that this is actually
# included from the main Makefile, and that pathnames should be
# accordingly.
#

KLIBCARCHOBJS = \
	arch/$(KLIBCARCH)/setjmp.o \
	arch/$(KLIBCARCH)/syscall.o

KLIBCARCHSOOBJS = $(patsubst %.o,%.lo,$(KLIBCARCHOBJS))

INTERP_O = interp1.o

interp.o: interp1.o klibc.got
	$(LD) $(KLIBCLDFLAGS) -r -o $@ interp1.o klibc.got

klibc.got: $(SOHASH)
	$(OBJCOPY) -j .got $< $@

archclean:
	rm -f klibc.got
