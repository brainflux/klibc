SUBDIRS = klibc ash utils gzip

all:

%:
	@set -e; for d in $(SUBDIRS); do $(MAKE) -C $$d $@; done
