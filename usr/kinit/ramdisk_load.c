#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include "kinit.h"
#include "fstype.h"
#include "zlib.h"

#define BUF_SZ		65536

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
		

