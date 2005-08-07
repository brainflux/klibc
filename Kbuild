#
# Kbuild file for klibc
#
.PHONY: $(obj)/all
always := all

$(obj)/all:
	$(Q)$(MAKE) $(klibc)=scripts/basic
	$(Q)$(MAKE) $(klibc)=klibc
	$(Q)$(MAKE) $(klibc)=usr/kinit
	$(Q)$(MAKE) $(klibc)=ash
	$(Q)$(MAKE) $(klibc)=utils
	$(Q)$(MAKE) $(klibc)=gzip


# Directories to visit during clean and install
subdir- := klibc ash utils gzip usr/kinit
