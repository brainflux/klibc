SUBDIRS = klibc ash ipconfig utils kinit gzip

all:

%:
	@set -e; for d in $(SUBDIRS); do $(MAKE) -C $$d $@; done
