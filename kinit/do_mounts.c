#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "kinit.h"

#define BUF_SZ		4096
#define Root_NFS	0xff

static const char *progname = "VFS";
static const int do_devfs; // FIXME

/* Find dev_t for e.g. "hda,NULL" or "hdb,2" */
static dev_t
try_name(char *name, int part)
{
	char path[BUF_SZ];
	char buf[BUF_SZ];
	int range;
	dev_t res;
	char *s;
	int len;
	int fd;

	/* read device number from /sys/block/.../dev */
	snprintf(path, sizeof(path), "/sys/block/%s/dev", name);
	fd = open(path, 0, 0);
	if (fd < 0)
		goto fail;
	len = read(fd, buf, BUF_SZ);
	close(fd);

	if (len <= 0 || len == BUF_SZ || buf[len - 1] != '\n')
		goto fail;
	buf[len - 1] = '\0';
	res = (dev_t) strtoul(buf, &s, 16);
	if (*s)
		goto fail;

	/* if it's there and we are not looking for a partition - that's it */
	if (!part)
		return res;

	/* otherwise read range from .../range */
	snprintf(path, sizeof(path), "/sys/block/%s/range", name);
	fd = open(path, 0, 0);
	if (fd < 0)
		goto fail;
	len = read(fd, buf, 32);
	close(fd);
	if (len <= 0 || len == 32 || buf[len - 1] != '\n')
		goto fail;
	buf[len - 1] = '\0';
	range = strtoul(buf, &s, 10);
	if (*s)
		goto fail;

	/* if partition is within range - we got it */
	if (part < range)
		return res + part;

fail:
	return (dev_t) 0;
}

/*
 *	Convert a name into device number.  We accept the following variants:
 *
 *	1) device number in hexadecimal	represents itself
 *	2) /dev/nfs represents Root_NFS
 *	3) /dev/<disk_name> represents the device number of disk
 *	4) /dev/<disk_name><decimal> represents the device number
 *         of partition - device number of disk plus the partition number
 *	5) /dev/<disk_name>p<decimal> - same as the above, that form is
 *	   used when disk name of partitioned disk ends on a digit.
 *
 *	If name doesn't have fall into the categories above, we return 0.
 *	Driverfs is used to check if something is a disk name - it has
 *	all known disks under bus/block/devices.  If the disk name
 *	contains slashes, name of driverfs node has them replaced with
 *	dots.  try_name() does the actual checks, assuming that driverfs
 *	is mounted on rootfs /sys.
 */

static dev_t
name_to_dev_t(const char *name)
{
	char *p;
	dev_t res = 0;
	char s[32];
	int part;

	if (strncmp(name, "/dev/", 5) != 0) {
		res = (dev_t) strtoul(name, &p, 16);
		if (*p)
			goto fail;
		goto done;
	}
	name += 5;
	res = Root_NFS;
	if (strcmp(name, "nfs") == 0)
		goto done;

	if (strlen(name) > 31)
		goto fail;
	strcpy(s, name);

	for (p = s; *p; p++)
		if (*p == '/')
			*p = '.';
	res = try_name(s, 0);
	if (res)
		goto done;

	while (p > s && isdigit(p[-1]))
		p--;
	if (p == s || !*p || *p == '0')
		goto fail;
	part = strtoul(p, NULL, 10);
	*p = '\0';
	res = try_name(s, part);
	if (res)
		goto done;

	if (p < s + 2 || !isdigit(p[-2]) || p[-1] != 'p')
		goto fail;
	p[-1] = '\0';
	res = try_name(s, part);
 done:
	return res;
 fail:
	res = (dev_t) 0;
	goto done;
}

static int
find_in_devfs(char *path, dev_t dev)
{
	(void) path;
	(void) dev;
	
#ifdef CONFIG_DEVFS_FS
	// Leftover crap from the kernel.  Ugh.

	struct stat buf;
	char *end = path + strlen(path);
	int rest = path + 64 - end;
	int size;
	char *p = read_dir(path, &size);
	char *s;

	if (!p)
		return -1;
	for (s = p; s < p + size; s += ((struct linux_dirent64 *)s)->d_reclen) {
		struct linux_dirent64 *d = (struct linux_dirent64 *)s;
		if (strlen(d->d_name) + 2 > rest)
			continue;
		switch (d->d_type) {
			case DT_BLK:
				sprintf(end, "/%s", d->d_name);
				if (sys_newstat(path, &buf) < 0)
					break;
				if (!S_ISBLK(buf.st_mode))
					break;
				if (buf.st_rdev != dev)
					break;
				kfree(p);
				return 0;
			case DT_DIR:
				if (strcmp(d->d_name, ".") == 0)
					break;
				if (strcmp(d->d_name, "..") == 0)
					break;
				sprintf(end, "/%s", d->d_name);
				if (find_in_devfs(path, dev) < 0)
					break;
				kfree(p);
				return 0;
		}
	}
	kfree(p);
#endif
	return -1;
}

/* Create the device node "name" */
static int
create_dev(const char *name, dev_t dev, const char *devfs_name)
{
	char path[BUF_SZ];

	unlink(name);
	if (!do_devfs)
		return mknod(name, S_IFBLK|0600, dev);

	if (devfs_name && devfs_name[0]) {
		if (strncmp(devfs_name, "/dev/", 5) == 0)
			devfs_name += 5;
		snprintf(path, BUF_SZ, "/dev/%s", devfs_name);
		if (access(path, 0) == 0)
			return symlink(devfs_name, name);
	}
	if (!dev)
		return -1;

	strcpy(path, "/dev");
	if (find_in_devfs(path, dev) < 0)
		return -1;

	return symlink(path + 5, name);
}

/* get a list of all filesystems to try to mount root with */
static void
get_fs_names(int argc, char *argv[], char *buf)
{
	char *s, *p;
	char line[BUF_SZ];
	const char *root_fs_names;
	FILE *fp;

	root_fs_names = get_arg(argc, argv, "rootfstype=");
	if (root_fs_names) {
		strcpy(buf, root_fs_names);
		for (s = buf; *s; s++) {
			if (*s == ',')
				*s = 0;
		}
	} else {
		buf[0] = 0;
		if ((fp = fopen("/proc/filesystems", "r")) == NULL)
			goto done;

		s = buf;
		while (fgets(line, BUF_SZ, fp)) {
			if (line[0] != '\t')
				continue;
			p = strchr(line, '\n');
			if (!p)
				continue;

			*p = 0;
			strcpy(s, line + 1);
			s += strlen(line + 1) + 1;
		}
	}
 done:
	return;
}

/* mount the root filesystem from a block device */
static int
mount_block_root(int argc, char *argv[],
		 dev_t root_dev, const char *root_dev_name, int flags)
{
	char fs_names[BUF_SZ];
	const char *data;
	char *p;

	data = get_arg(argc, argv, "rootflags=");

	create_dev("/dev/root", root_dev, root_dev_name);

	get_fs_names(argc, argv, fs_names);
retry:
	for (p = fs_names; *p; p += strlen(p)+1) {
		if (mount("/dev/root", "/root", p, flags, data) == 0)
			goto out;

		switch (errno) {
			case -EACCES:
				flags |= MS_RDONLY;
				goto retry;
			case -EINVAL:
				continue;
		}
	        /*
		 * Allow the user to distinguish between failed open
		 * and bad superblock on root device.
		 */
		fprintf(stderr, "%s: Cannot open root device \"%04x\"\n",
			progname, root_dev);
		return -errno;
	}
	fprintf(stderr, "%s: Unable to mount root fs on \"%04x\"\n",
		progname, root_dev);
	return -ESRCH;

out:
	printf("%s: Mounted root (%s filesystem)%s.\n",
	       progname, p, (flags & MS_RDONLY) ? " readonly" : "");
	return 0;
}

int
do_mounts(int argc, char *argv[])
{
	const char *root_dev_name = get_arg(argc, argv, "root=");
	int flags = MS_RDONLY | MS_VERBOSE;
	dev_t root_dev = 0;
	int ret;

	if (root_dev_name) {
		root_dev = name_to_dev_t(root_dev_name);
		if (strncmp(root_dev_name, "/dev/", 5) == 0) {
			root_dev_name += 5;
		}
	}

	if (get_arg(argc, argv, "ro")) {
		flags |= MS_RDONLY;
	}
	if (get_arg(argc, argv, "rw")) {
		flags &= ~MS_RDONLY;
	}

	if (root_dev == Root_NFS) {
		ret = mount_nfs_root(argc, argv, flags);
	} else {
		ret = mount_block_root(argc, argv, root_dev, root_dev_name, flags);
	}

	if (ret == 0) {
		chdir("/root");
	}
	
	return ret;
}
