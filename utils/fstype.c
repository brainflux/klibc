/*
 * fstype.c, by rmk
 *
 * Detect filesystem type (on stdin) and output strings for two
 * environment variables:
 *  FSTYPE - filesystem type
 *  FSSIZE - filesystem size (if known)
 *
 * We currently detect (in order):
 *  gzip, cramfs, romfs, minix, ext3, ext2, reiserfs
 *
 * MINIX, ext3 and Reiserfs bits are currently untested.
 */
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <asm/byteorder.h>

#include <linux/romfs_fs.h>
#include <linux/cramfs_fs.h>
#include <linux/minix_fs.h>
#include <linux/ext2_fs.h>
#include <linux/ext3_fs.h>

/*
 * reiserfs_fs.h is too sick to include directly.
 * Use a cleaned up version.
 */
#include "reiserfs_fs.h"

#define htonl __cpu_to_be32

char *progname;

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(x[0]))

#define BLOCK_SIZE_BITS 10
#define BLOCK_SIZE	(1 << BLOCK_SIZE_BITS)
#define BYTES_TO_BLOCKS(x) (((x) + (BLOCK_SIZE - 1)) >> BLOCK_SIZE_BITS)

static int fstype(const char *type, unsigned long size)
{
	fprintf(stdout, "FSTYPE=%s\nFSSIZE=%lu\n", type, size);
	return 0;
}

static int readfile(int fd, off_t off, void *buf, size_t size)
{
	int ret;

	if (lseek(fd, off, SEEK_SET) == -1) {
		perror("lseek");
		return 1;
	}

	ret = read(fd, buf, size);
	if (ret == -1) {
		perror("read");
		return 1;
	}
	if (ret != BLOCK_SIZE) {
		fprintf(stderr, "%s: short read\n", progname);
		return 1;
	}
	return 0;
}

static int gzip_image(const unsigned char *buf, unsigned long *blocks)
{
	if (buf[0] == 037 && (buf[1] == 0213 || buf[1] == 0236)) {
		*blocks = 0;
		return 1;
	}
	return 0;
}

static int cramfs_image(const unsigned char *buf, unsigned long *blocks)
{
	const struct cramfs_super *sb =
		(const struct cramfs_super *)buf;

	if (sb->magic == CRAMFS_MAGIC) {
		if (sb->flags & CRAMFS_FLAG_FSID_VERSION_2)
			*blocks = sb->fsid.blocks;
		else
			*blocks = 0;
		return 1;
	}
	return 0;
}

static int romfs_image(const unsigned char *buf, unsigned long *blocks)
{
        const struct romfs_super_block *sb =
		(const struct romfs_super_block *)buf;

	if (sb->word0 == ROMSB_WORD0 && sb->word1 == ROMSB_WORD1) {
		*blocks = (unsigned long)BYTES_TO_BLOCKS(__be32_to_cpu(sb->size));
		return 1;
	}
	return 0;
}

static int minix_image(const unsigned char *buf, unsigned long *blocks)
{
	const struct minix_super_block *sb =
		(const struct minix_super_block *)buf;

	if (sb->s_magic == MINIX_SUPER_MAGIC ||
	    sb->s_magic == MINIX_SUPER_MAGIC2) {
		*blocks = sb->s_nzones << sb->s_log_zone_size;
		return 1;
	}
	return 0;
}

static int ext3_image(const unsigned char *buf, unsigned long *blocks)
{
	const struct ext3_super_block *sb =
		(const struct ext3_super_block *)buf;

	if (sb->s_magic == __cpu_to_le16(EXT2_SUPER_MAGIC) &&
	    sb->s_feature_compat & __cpu_to_le32(EXT3_FEATURE_COMPAT_HAS_JOURNAL)) {
		*blocks = __le32_to_cpu(sb->s_blocks_count);
		return 1;
	}
	return 0;
}

static int ext2_image(const unsigned char *buf, unsigned long *blocks)
{
	const struct ext2_super_block *sb =
		(const struct ext2_super_block *)buf;

	if (sb->s_magic == __cpu_to_le16(EXT2_SUPER_MAGIC)) {
		*blocks = __le32_to_cpu(sb->s_blocks_count);
		return 1;
	}
	return 0;
}

static int reiserfs_image(const unsigned char *buf, unsigned long *blocks)
{
	const struct reiserfs_super_block *sb =
		(const struct reiserfs_super_block *)buf;

	if (memcmp(REISERFS_MAGIC(sb), REISERFS_SUPER_MAGIC_STRING,
		   sizeof(REISERFS_SUPER_MAGIC_STRING) - 1) == 0 ||
	    memcmp(REISERFS_MAGIC(sb), REISER2FS_SUPER_MAGIC_STRING,
		   sizeof(REISER2FS_SUPER_MAGIC_STRING) - 1) == 0 ||
	    memcmp(REISERFS_MAGIC(sb), REISER2FS_JR_SUPER_MAGIC_STRING,
		   sizeof(REISER2FS_JR_SUPER_MAGIC_STRING) - 1) == 0) {
		*blocks = REISERFS_BLOCK_COUNT(sb) *
			  (REISERFS_BLOCKSIZE(sb) / BLOCK_SIZE);
		return 1;
	}
	return 0;
}

struct imagetype {
	off_t		block;
	const char	name[12];
	int		(*identify)(const unsigned char *, unsigned long *);
};

static struct imagetype images[] = {
	{ 0,	"gzip",		gzip_image	},
	{ 0,	"cramfs",	cramfs_image	},
	{ 0,	"romfs",	romfs_image	},
	{ 1,	"minix",	minix_image	},
	{ 1,	"ext3",		ext3_image	},
	{ 1,	"ext2",		ext2_image	},
	{ 8,	"reiserfs",	reiserfs_image	},
	{ 64,	"reiserfs",	reiserfs_image	}
};

int main(int argc, char *argv[])
{
	unsigned char buf[BLOCK_SIZE];
	unsigned long size = 0;
	off_t cur_block = (off_t)-1;
	unsigned int i;
	int ret;

	progname = argv[0];

	if (argc != 1) {
		fprintf(stderr, "Usage: %s < file\n", progname);
		return 1;
	}

	for (i = 0; i < ARRAY_SIZE(images); i++) {
		if (cur_block != images[i].block) {
			/*
			 * Read block.
			 */
			cur_block = images[i].block;
			ret = readfile(0, cur_block * BLOCK_SIZE, buf,
				       BLOCK_SIZE);
			if (ret != 0)
				return ret;
		}

		if (images[i].identify(buf, &size))
			return fstype(images[i].name, size);
	}

	fstype("unknown", 0);
	return 0;
}
