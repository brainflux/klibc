/*
 * kinit/initrd.c
 *
 * Handle initrd, thus putting the backwards into backwards compatible
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include "do_mounts.h"
#include "kinit.h"

#define BUF_SIZE	65536	/* Should be a power of 2 */

/*
 * Copy the initrd to /dev/ram0, copy from the end to the beginning
 * to avoid taking 2x the memory.
 */
int rd_copy_image(int ffd)
{
	int dfd = open("/dev/ram0", O_RDWR);
	char buffer[BUF_SIZE];
	struct stat st;
	off_t bytes;
	int rv = -1;

	if ( ffd < 0 || dfd < 0 )
		goto barf;

	if ( fstat(ffd, &st) || !S_ISREG(st.st_mode) ||
	     (bytes = st.st_size) == 0 )
		goto barf;

	while ( bytes ) {
		ssize_t blocksize = ((bytes-1) & ~(BUF_SIZE-1))+1;
		off_t offset = bytes-blocksize;

		if ( pread(ffd, buffer, blocksize, offset) != blocksize ||
		     pwrite(dfd, buffer, blocksize, offset) != blocksize )
			goto barf;

		ftruncate(ffd, offset); /* Free up memory */
		bytes = offset;
	}
	rv = 0;			/* Done! */

 barf:
	close(ffd);
	close(dfd);
	return rv;
}

/*
 * Run /linuxrc, for emulation of old-style initrd
 */
static int
run_linuxrc(int argc, char *argv[], dev_t root_dev)
{
	int root_fd, old_fd;
	pid_t pid;
	long realroot = Root_RAM0;
	const char *ramdisk_name = "/dev/ram0";
	FILE *fp;

	if ( !mount_block(ramdisk_name, "/", NULL, MS_VERBOSE, NULL) )
		return -errno;

	/* Write the current "real root device" out to procfs */
	fp = fopen("/proc/sys/kernel/real-root-dev", "w");
	fprintf(fp, "%lu", (unsigned long)root_dev);
	fclose(fp);

	mkdir("/old", 0700);
	root_fd = open_cloexec("/", O_RDONLY|O_DIRECTORY, 0);
	old_fd = open_cloexec("/old", O_RDONLY|O_DIRECTORY, 0);

	if ( root_fd < 0 || old_fd < 0 )
		return -errno;

	if ( chdir("/root") ||
	     mount(".", "/", NULL, MS_MOVE, NULL) ||
	     chroot(".") )
		return -errno;

	pid = fork();
	if ( pid == 0 ) {
		setsid();
		/* Looks like linuxrc doesn't get the init environment or parameters.
		   Weird, but so is the whole linuxrc bit. */
		execl("/linuxrc", "linuxrc", NULL);
		_exit(255);
	} else if ( pid > 0 ) {
		while ( waitpid(pid, NULL, 0) != pid );
	} else {
		return -errno;
	}

	if ( fchdir(old_fd) ||
	     mount("/", ".", NULL, MS_MOVE, NULL) ||
	     fchdir(root_fd) ||
	     chroot(".") )
		return -errno;

	close(root_fd);
	close(old_fd);

	getintfile("/proc/sys/kernel/real-root-dev", &realroot);
	mount_root(argc, argv, (dev_t)realroot, NULL);

	/* If /root/initrd exists, move the initrd there, otherwise discard */
	if ( !mount("/old", "/root/initrd", NULL, MS_MOVE, NULL) ) {
		/* We're good */
	} else {
		int olddev = open(ramdisk_name, O_RDWR);
		umount2("/old", MNT_DETACH);
		if ( olddev < 0 ||
		     ioctl(olddev, BLKFLSBUF, (long)0) ||
		     close(olddev) ) {
			fprintf(stderr, "%s: Cannot flush initrd contents\n", progname);
		}
	}

	return 0;
}


int initrd_load(int argc, char *argv[], dev_t root_dev)
{
	int initrd_fd;

	initrd_fd = open("/initrd.image", O_RDWR);
	if ( initrd_fd < 0 )
		return 0;

	create_dev("/dev/ram0", Root_RAM0, NULL);

	if ( rd_copy_image(initrd_fd) ||
	     unlink("/initrd.image") )
		return 0;

	if ( root_dev != Root_RAM0 ) {
		run_linuxrc(argc, argv, root_dev);
		return 1;
	}

	return 0;
}
