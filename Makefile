SUBDIRS = klibc ash ipconfig nfsmount utils kinit gzip

all:

%:
	@set -e; for d in $(SUBDIRS); do $(MAKE) -C $$d $@; done
