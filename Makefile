SUBDIRS = klibc ash ipconfig nfsmount utils kinit gzip

all:

rpmbuild = $(shell which rpmbuild 2>/dev/null || which rpm)

.PHONY: rpm
rpm:
	+$(rpmbuild) -bb klibc.spec

%:
	@set -e; for d in $(SUBDIRS); do $(MAKE) -C $$d $@; done
