SUBDIRS = klibc ash utils gzip ipconfig

all:

%:
	@set -e; for d in $(SUBDIRS); do $(MAKE) -C $$d $@; done
