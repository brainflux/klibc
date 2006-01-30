#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <alloca.h>
#include <inttypes.h>

#include "do_mounts.h"
#include "kinit.h"
#include "fstype.h"
#include "zlib.h"

#define BUF_SZ		65536

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
 *	6) an actual block device node in the initramfs filesystem
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
	char *s;
	int part;
	struct stat st;
	int len;

	if ( name[0] == '/' && !stat(name, &st) && S_ISBLK(st.st_mode) )
		return st.st_rdev;
	
	if ( strncmp(name, "/dev/", 5) ) {
		res = (dev_t) strtoul(name, &p, 16);
		if (*p)
			return 0;
		return res;
	}
	name += 5;
	if (strcmp(name, "nfs") == 0)
		return Root_NFS;

	len = strlen(name);
	s = alloca(len+1);
	memcpy(s, name, len+1);

	for (p = s; *p; p++)
		if (*p == '/')
			*p = '.';
	res = try_name(s, 0);
	if (res)
		return res;

	while (p > s && isdigit(p[-1]))
		p--;
	if (p == s || !*p || *p == '0')
		goto fail;
	part = strtoul(p, NULL, 10);
	*p = '\0';
	res = try_name(s, part);
	if (res)
		return res;

	if (p < s + 2 || !isdigit(p[-2]) || p[-1] != 'p')
		goto fail;
	p[-1] = '\0';
	res = try_name(s, part);
	return res;

 fail:
	return (dev_t)0;
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
int
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

/* mount a filesystem, possibly trying a set of different types */
const char *
mount_block(const char *source, const char *target,
	    const char *type, unsigned long flags,
	    const void *data)
{
	char *fslist, *p, *ep;
	const char *rp;
	ssize_t fsbytes;

	if ( type ) {
		int rv = mount(source, target, type, flags, data);
		/* Mount readonly if necessary */
		if ( rv == -1 && errno == EACCES && !(flags & MS_RDONLY) )
			rv = mount(source, target, type, flags|MS_RDONLY, data);
		return rv ? NULL : type;
	}

	fsbytes = readfile("/proc/filesystems", &fslist);

	errno = EINVAL;
	if ( fsbytes < 0 )
		return NULL;

	p = fslist;
	ep = fslist+fsbytes;

	rp = NULL;

	while ( p < ep ) {
		type = p;
		p = strchr(p, '\n');
		if (!p)
			break;
		*p++ = '\0';
		if (*type != '\t')/* We can't mount a block device as a "nodev" fs */
			continue;
		
		type++;
		rp = mount_block(source, target, type, flags, data);
		if ( rp )
			break;
		if ( errno != EINVAL )
			break;
	}

	free(fslist);
	return rp;
}

/* mount the root filesystem from a block device */
static int
mount_block_root(int argc, char *argv[], dev_t root_dev,
		 const char *root_dev_name, unsigned long flags)
{
	const char *data, *type;
	const char *rp;

	data = get_arg(argc, argv, "rootflags=");
	create_dev("/dev/root", root_dev, root_dev_name);

	errno = 0;

	type = get_arg(argc, argv, "rootfstype=");
	if ( type ) {
		if ( (rp = mount_block("/dev/root", "/root", type, flags, data)) )
			goto ok;
		if ( errno != EINVAL )
			goto bad;
	}
	
	if ( !errno && (rp = mount_block("/dev/root", "/root", NULL, flags, data)) )
		goto ok;

 bad:
	if ( errno != EINVAL ) {
	        /*
		 * Allow the user to distinguish between failed open
		 * and bad superblock on root device.
		 */
		fprintf(stderr, "%s: Cannot open root device \"%04x\"\n",
			progname, root_dev);
		return -errno;
	} else {
		fprintf(stderr, "%s: Unable to mount root fs on \"%04x\"\n",
			progname, root_dev);
		return -ESRCH;
	}

ok:
	printf("%s: Mounted root (%s filesystem)%s.\n",
	       progname, rp, (flags & MS_RDONLY) ? " readonly" : "");
	return 0;
}

int
mount_root(int argc, char *argv[], dev_t root_dev, const char *root_dev_name)
{
	unsigned long flags = MS_RDONLY|MS_VERBOSE;
	int ret;

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

static void
md_run_setup(void)
{
	/* Muck around with md device(s) if necessary */
}

static int
ramdisk_load(int argc, char *argv[], dev_t root_dev)
{
        const char *arg_prompt_ramdisk = 
		get_arg(argc, argv, "prompt_ramdisk=");
	const char *arg_ramdisk_blocksize =
		get_arg(argc, argv, "ramdisk_blocksize=");
	const char *arg_ramdisk_start =
		get_arg(argc, argv, "ramdisk_start=");
	int prompt_ramdisk =
		arg_prompt_ramdisk ? atoi(arg_prompt_ramdisk) : 0;
	int ramdisk_blocksize =
		arg_ramdisk_blocksize ? atoi(arg_ramdisk_blocksize) : 512;
	off_t ramdisk_start =
		arg_ramdisk_start
		? strtoumax(arg_ramdisk_start, NULL, 10)*ramdisk_blocksize
		: 0;
	int rfd;
	FILE *wfd;
	char buf[BUF_SZ];
	const char *fstype;
	unsigned long long fssize;
	int is_gzip;
	ssize_t bytes = 0;

	if (prompt_ramdisk) {
		fprintf(stderr, "Please insert disk for ramdisk and press Enter...\n");
		getc(stdin);
	}

	/* XXX: This should be better error checked. */
	
	mknod("/dev/rddev", S_IFBLK|0400, root_dev);
	mknod("/dev/ram0", S_IFBLK|0600, Root_RAM0);
	rfd = open("/dev/rddev", O_RDONLY);
	wfd = fopen("/dev/ram0", "w");
	
	/* Check filesystem type */
	if (identify_fs(rfd, &fstype, &fssize, ramdisk_start) ||
	    (fssize == 0 && !(is_gzip = !strcmp(fstype, "gzip")))) {
		fprintf(stderr, "Failure loading ramdisk: unknown filesystem type\n");
		return 0;
	}

	lseek(rfd, ramdisk_start, SEEK_SET);

	fprintf(stderr, "Loading ramdisk...");
	if (is_gzip) {
		gzFile gzf = gzdopen(rfd, "r");

		while ( (bytes = gzread(gzf, buf, BUF_SZ)) ) {
			if (bytes < 0)
				break;
			_fwrite(buf, bytes, wfd);
			putc('.', stderr);
		}

		gzclose(gzf);
	} else {
		while (fssize) {
			bytes = (fssize > BUF_SZ) ? BUF_SZ : (int)fssize;
			bytes = read(rfd, buf, bytes);
			if (bytes < 0)
				break;
			_fwrite(buf, bytes, wfd);
			putc('.', stderr);
		}
		
		close(rfd);
	}

	putc('\n', stderr);

	if (bytes < 0) {
		perror("Failure loading ramdisk");
		return 0;
	}
			
	return 1;
}
		

int do_mounts(int argc, char *argv[])
{
	const char *root_dev_name = get_arg(argc, argv, "root=");
	const char *root_delay = get_arg(argc, argv, "rootdelay=");
	const char *load_ramdisk = get_arg(argc, argv, "load_ramdisk=");
	dev_t root_dev = 0;

	if (root_delay) {
		int delay = atoi(root_delay);
		fprintf(stderr, "Waiting %d s before mounting root device...\n", delay);
		sleep(delay);
	}

	md_run_setup();

	if (root_dev_name) {
		root_dev = name_to_dev_t(root_dev_name);
		if (strncmp(root_dev_name, "/dev/", 5) == 0) {
			root_dev_name += 5;
		}
	} else if (get_arg(argc, argv, "nfsroot=") ||
		   get_arg(argc, argv, "nfsaddrs=")) {
		root_dev = Root_NFS;
	}

	if ( initrd_load(argc, argv, root_dev) )
		return 0;

	if ( load_ramdisk && atoi(load_ramdisk) ) {
		if (ramdisk_load(argc, argv, root_dev))
			root_dev = Root_RAM0;
	}

	return mount_root(argc, argv, root_dev, root_dev_name);
}
