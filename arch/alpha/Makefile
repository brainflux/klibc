#
# arch/alpha/Makefile
#
# Special rules for the alpha make.  Note that this is actually included
# from the main Makefile, and that pathnames should be accordingly.
#

# Special CFLAGS for the divide code
DIVCFLAGS = $(CFLAGS) -O3 -fomit-frame-pointer -fcall-saved-1 -fcall-saved-2 \
	-fcall-saved-3 -fcall-saved-4 -fcall-saved-5 -fcall-saved-6 \
	-fcall-saved-7 -fcall-saved-8 -fcall-saved-22 -fcall-saved-23 \
	-fcall-saved-24 -fcall-saved-25 -fcall-saved-27

ARCHOBJS = \
	arch/$(ARCH)/__divqu.o \
	arch/$(ARCH)/__remqu.o \
	arch/$(ARCH)/__divqs.o \
	arch/$(ARCH)/__remqs.o \
	arch/$(ARCH)/__divlu.o \
	arch/$(ARCH)/__remlu.o \
	arch/$(ARCH)/__divls.o \
	arch/$(ARCH)/__remls.o
ARCHSOOBJS = \
	arch/$(ARCH)/__divqu.lo \
	arch/$(ARCH)/__remqu.lo \
	arch/$(ARCH)/__divqs.lo \
	arch/$(ARCH)/__remqs.lo \
	arch/$(ARCH)/__divlu.lo \
	arch/$(ARCH)/__remlu.lo \
	arch/$(ARCH)/__divls.lo \
	arch/$(ARCH)/__remls.lo

arch/$(ARCH)/%.s: arch/$(ARCH)/%.ss
	sed -e 's/\$0\b/$27/g' -e 's/\$24\b/$99/g' -e 's/\$16\b/$24/g' -e 's/\$17\b/$25/g' -e 's/\$99\b/$24/g' < $< > $@

arch/$(ARCH)/__divqu.ss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -DSIGNED=0 -DREM=0 -DBITS=64 -DNAME=__divqu -S -o $@ $<

arch/$(ARCH)/__remqu.ss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -DSIGNED=0 -DREM=1 -DBITS=64 -DNAME=__remqu -S -o $@ $<

arch/$(ARCH)/__divqs.ss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -DSIGNED=1 -DREM=0 -DBITS=64 -DNAME=__divqs -S -o $@ $<

arch/$(ARCH)/__remqs.ss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -DSIGNED=1 -DREM=1 -DBITS=64 -DNAME=__remqs -S -o $@ $<

arch/$(ARCH)/__divlu.ss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -DSIGNED=0 -DREM=0 -DBITS=32 -DNAME=__divlu -S -o $@ $<

arch/$(ARCH)/__remlu.ss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -DSIGNED=0 -DREM=1 -DBITS=32 -DNAME=__remlu -S -o $@ $<

arch/$(ARCH)/__divls.ss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -DSIGNED=1 -DREM=0 -DBITS=32 -DNAME=__divls -S -o $@ $<

arch/$(ARCH)/__remls.ss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -DSIGNED=1 -DREM=1 -DBITS=32 -DNAME=__remls -S -o $@ $<

arch/$(ARCH)/__divqu.lss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -fPIC -DSIGNED=0 -DREM=0 -DBITS=64 -DNAME=__divqu -S -o $@ $<

arch/$(ARCH)/__remqu.lss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -fPIC -DSIGNED=0 -DREM=1 -DBITS=64 -DNAME=__remqu -S -o $@ $<

arch/$(ARCH)/__divqs.lss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -fPIC -DSIGNED=1 -DREM=0 -DBITS=64 -DNAME=__divqs -S -o $@ $<

arch/$(ARCH)/__remqs.lss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -fPIC -DSIGNED=1 -DREM=1 -DBITS=64 -DNAME=__remqs -S -o $@ $<

arch/$(ARCH)/__divlu.lss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -fPIC -DSIGNED=0 -DREM=0 -DBITS=32 -DNAME=__divlu -S -o $@ $<

arch/$(ARCH)/__remlu.lss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -fPIC -DSIGNED=0 -DREM=1 -DBITS=32 -DNAME=__remlu -S -o $@ $<

arch/$(ARCH)/__divls.lss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -fPIC -DSIGNED=1 -DREM=0 -DBITS=32 -DNAME=__divls -S -o $@ $<

arch/$(ARCH)/__remls.lss: arch/$(ARCH)/divide.c
	$(CC) $(DIVCFLAGS) -fPIC -DSIGNED=1 -DREM=1 -DBITS=32 -DNAME=__remls -S -o $@ $<
