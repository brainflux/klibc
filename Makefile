VERSION := $(shell cat version)
SUBDIRS = klibc ash ipconfig nfsmount utils kinit gzip
SRCROOT = .

all: klcc

rpmbuild = $(shell which rpmbuild 2>/dev/null || which rpm)

klibc.spec: klibc.spec.in version
	sed -e 's/@@VERSION@@/$(VERSION)/g' < $< > $@

.PHONY: rpm
rpm: klibc.spec
	+$(rpmbuild) -bb klibc.spec --target=$(ARCH)

$(CROSS)klibc.config: Makefile
	rm -f $@
	echo 'ARCH=$(ARCH)' >> $@
	echo 'CROSS=$(CROSS)' >> $@
	echo "CC=$(shell bash -c 'type -p $(CC)')" >> $@
	echo "LD=$(shell bash -c 'type -p $(LD)')" >> $@
	echo 'REQFLAGS=$(filter-out -I%,$(REQFLAGS))' >> $@
	echo 'OPTFLAGS=$(OPTFLAGS)' >> $@
	echo 'LDFLAGS=$(LDFLAGS)' >> $@
	echo 'STRIP=$(STRIP)' >> $@
	echo 'EMAIN=$(EMAIN)' >> $@
	echo 'BITSIZE=$(BITSIZE)' >> $@
	echo 'INSTALLDIR=$(INSTALLDIR)' >> $@

$(CROSS)klcc: klcc.in $(CROSS)klibc.config makeklcc.pl
	$(PERL) makeklcc.pl klcc.in $(CROSS)klibc.config \
		$(shell bash -c 'type -p $(PERL)') > $@ || ( rm -f $@ ; exit 1 )
	chmod a+x $@

%: local-%
	@set -e; for d in $(SUBDIRS); do $(MAKE) -C $$d $@; done

local-clean:
	rm -f klibc.config klcc

local-spotless:
	rm -f klibc.spec *~ tags

local-install: $(CROSS)klcc
	mkdir -p $(INSTALLROOT)$(bindir)
	mkdir -p $(INSTALLROOT)$(libdir)
	mkdir -p $(INSTALLROOT)$(INSTALLDIR)/include
	mkdir -p $(INSTALLROOT)$(INSTALLDIR)/$(CROSS)lib
	mkdir -p $(INSTALLROOT)$(INSTALLDIR)/$(CROSS)bin
	cp -r $(KRNLSRC)/linux/include/. $(INSTALLDIR)/include/.
	cp -r $(KRNLOBJ)/linux/include/. $(INSTALLDIR)/include/.
	cp -r $(KRNLOBJ)/linux/include2/. $(INSTALLDIR)/include/.
	rm -rf $(INSTALLDIR)/include/asm-*
	cp -r include/. $(INSTALLDIR)/include/.
	$(INSTALL_EXEC) $(CROSS)klcc $(bindir)

-include MCONFIG
