/*
 * do_mounts.h
 */

#ifndef DO_MOUNTS_H
#define DO_MOUNTS_H

#include <sys/types.h>
#include <sys/sysmacros.h>
#include <sys/stat.h>

#define	Root_RAM0	makedev(1,0)
#define Root_NFS	makedev(0,255)

int
create_dev(const char *name, dev_t dev, const char *devfs_name);

dev_t
name_to_dev_t(const char *name);

const char *
mount_block(const char *source, const char *target,
	    const char *type, unsigned long flags,
	    const void *data);

int
mount_root(int argc, char *argv[], dev_t root_dev, const char *root_dev_name);

int do_mounts(int argc, char *argv[]);

int initrd_load(int argc, char *argv[], dev_t root_dev);

static inline dev_t bstat(const char *name)
{
	struct stat st;

	if (stat(name, &st))
		return 0;
	if (!S_ISBLK(st.st_mode))
		return 0;
	return st.st_rdev;
}

int load_ramdisk_compressed(int rfd, FILE *wfd, off_t ramdisk_start);

#endif /* DO_MOUNTS_H */
