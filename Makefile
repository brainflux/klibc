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
	echo 'CC=$(CC)' >> $@
	echo 'LD=$(LD)' >> $@
	echo 'REQFLAGS=$(filter-out -I%,$(REQFLAGS))' >> $@
	echo 'OPTFLAGS=$(OPTFLAGS)' >> $@
	echo 'LDFLAGS=$(LDFLAGS)' >> $@
	echo 'STRIP=$(STRIP)' >> $@
	echo 'EMAIN=$(EMAIN)' >> $@
	echo 'BITSIZE=$(BITSIZE)' >> $@
	echo 'INSTALLDIR=$(INSTALLDIR)' >> $@

$(CROSS)klcc: klcc.in $(CROSS)klibc.config makeklcc
	$(PERL) makeklcc klcc.in $(CROSS)klibc.config \
		$(shell sh -c 'type -p $(PERL)') > $@ || ( rm -f $@ ; exit 1 )
	chmod a+x $@

%:
	@set -e; for d in $(SUBDIRS); do $(MAKE) -C $$d $@; done

clean:
	rm -f klibc.config klcc
	@set -e; for d in $(SUBDIRS); do $(MAKE) -C $$d $@; done

spotless:
	@set -e; for d in $(SUBDIRS); do $(MAKE) -C $$d $@; done
	rm -f klibc.spec *~ tags

-include MCONFIG
