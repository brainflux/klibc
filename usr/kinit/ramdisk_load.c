#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <linux/fs.h>

#include "kinit.h"
#include "do_mounts.h"
#include "fstype.h"
#include "zlib.h"

#define BUF_SZ		65536

static int
load_ramdisk_compressed(int rfd, FILE *wfd, off_t ramdisk_start)
{
	uint64_t ramdisk_size;
	int disk = 1;
	ssize_t bytes;
	int rv;
	unsigned char in_buf[BUF_SZ], out_buf[BUF_SZ];

	z_stream zs;

	zs.zalloc = Z_NULL;	/* Use malloc() */
	zs.zfree = Z_NULL;	/* Use free() */
	zs.next_in = Z_NULL;	/* No data read yet */
	zs.avail_in = 0;
	zs.next_out = out_buf;
	zs.avail_out = BUF_SZ;

	if (inflateInit(&zs) != Z_OK)
		goto err1;

	/* Set to the size of the medium, or "infinite" */
	if (ioctl(rfd, BLKGETSIZE64, &ramdisk_size))
		ramdisk_size = ~(uint64_t)0;

	do {
		/* Purge the output preferentially over reading new
		   input, so we don't end up overrunning the input by
		   accident and demanding a new disk which doesn't
		   exist... */
		if (zs.avail_out == 0) {
			_fwrite(out_buf, BUF_SZ, wfd);
			zs.next_out = out_buf;
			zs.avail_out = BUF_SZ;
			putc('.', stderr);
		} else if (zs.avail_in == 0) {
			if (ramdisk_start >= ramdisk_size) {
				fprintf(stderr, "\nPlease insert disk %d for ramdisk and press Enter...", ++disk);
				while (getc(stdin) != '\n')
					;
				if (ioctl(rfd, BLKGETSIZE64, &ramdisk_size))
					ramdisk_size = ~(uint64_t)0;
				ramdisk_start = 0;
			}
			do {
				bytes = min(ramdisk_start-ramdisk_size, (uint64_t)BUF_SZ);
				bytes = pread(rfd, in_buf, bytes, ramdisk_start);
			} while (bytes == -1 && errno == EINTR);
			if (bytes <= 0)
				goto err2;
			ramdisk_start += bytes;
			zs.next_in = in_buf;
			zs.avail_in = bytes;
		}
		rv = inflate(&zs, Z_NO_FLUSH);
	} while (rv == Z_OK || rv == Z_BUF_ERROR);

	if (rv != Z_STREAM_END)
		goto err2;

	/* Write the last */
	_fwrite(out_buf, BUF_SZ-zs.avail_out, wfd);

	inflateEnd(&zs);
	return 0;
	
err2:
	inflateEnd(&zs);
err1:
	return 1;
}


static int
load_ramdisk_raw(int rfd, FILE *wfd, off_t ramdisk_start, unsigned long long fssize)
{
	uint64_t ramdisk_size;
	int disk = 1;
	ssize_t bytes;
	unsigned char buf[BUF_SZ];

	/* Set to the size of the medium, or "infinite" */
	if (ioctl(rfd, BLKGETSIZE64, &ramdisk_size))
		ramdisk_size = ~(uint64_t)0;

	while (fssize) {
		if (ramdisk_start >= ramdisk_size) {
			fprintf(stderr, "\nPlease insert disk %d for ramdisk and press Enter...", ++disk);
			while (getc(stdin) != '\n')
				;
			if (ioctl(rfd, BLKGETSIZE64, &ramdisk_size))
				ramdisk_size = ~(uint64_t)0;
			lseek(rfd, 0L, SEEK_SET);
			ramdisk_start = 0;
		}
		
		do {
			bytes = min(ramdisk_start-ramdisk_size,
				    min((uint64_t)fssize, (uint64_t)BUF_SZ));
			bytes = pread(rfd, buf, bytes, ramdisk_start);
		} while (bytes == -1 && errno == EINTR);
		if (bytes <= 0)
			break;
		_fwrite(buf, bytes, wfd);
		putc('.', stderr);
		ramdisk_start += bytes;
	}

	return !!fssize;
}


int
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
	const char *fstype;
	unsigned long long fssize;
	int is_gzip = 0;
	int err;

	if (prompt_ramdisk) {
		fprintf(stderr, "Please insert disk for ramdisk and press Enter...");
		while (getc(stdin) != '\n');
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

	fprintf(stderr, "Loading ramdisk...");

	if (is_gzip)
		err = load_ramdisk_compressed(rfd, wfd, ramdisk_start);
	else
		err = load_ramdisk_raw(rfd, wfd, ramdisk_start, fssize);

	close(rfd);
	fclose(wfd);
	
	putc('\n', stderr);

	if (err) {
		perror("Failure loading ramdisk");
		return 0;
	}
			
	return 1;
}
		

