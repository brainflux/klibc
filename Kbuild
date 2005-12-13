#
# Kbuild file for klibc
#
.PHONY: $(obj)/all
always := all

$(obj)/all:
	$(Q)$(MAKE) $(klibc)=scripts/basic
	$(Q)$(MAKE) $(klibc)=klibc
	$(Q)$(MAKE) $(klibc)=usr/kinit
	$(Q)$(MAKE) $(klibc)=dash
	$(Q)$(MAKE) $(klibc)=utils
	$(Q)$(MAKE) $(klibc)=gzip


# Directories to visit during clean and install
subdir- := scripts/basic klcc klibc dash utils gzip usr/kinit klibc/tests
