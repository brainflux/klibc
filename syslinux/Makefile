SYSDIR = $(wildcard syslinux-[0-9]*)

all:
	$(MAKE) -C $(SYSDIR) installer
	$(MAKE) -f Makefile.main

clean:
	$(MAKE) -f Makefile.main clean

spotless:
	$(MAKE) -C $(SYSDIR) clean
	$(MAKE) -f Makefile.main spotless

