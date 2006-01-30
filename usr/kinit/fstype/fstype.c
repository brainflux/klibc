/*
 * fstype.c, by rmk
 *
 * Detect filesystem type (on stdin) and output strings for two
 * environment variables:
 *  FSTYPE - filesystem type
 *  FSSIZE - filesystem size (if known)
 *
 * We currently detect (in order):
 *  gzip, cramfs, romfs, xfs, minix, ext3, ext2, reiserfs, jfs
 *
 * MINIX, ext3 and Reiserfs bits are currently untested.
 */

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <endian.h>
#include <netinet/in.h>
#include <sys/vfs.h>

#define cpu_to_be32(x) __cpu_to_be32(x)	/* Needed by romfs_fs.h */

#include "romfs_fs.h"
#include "cramfs_fs.h"
#include "minix_fs.h"
#include "ext2_fs.h"
#include "ext3_fs.h"
#include "xfs_sb.h"

/*
 * Slightly cleaned up version of jfs_superblock to
 * avoid pulling in other kernel header files.
 */
#include "jfs_superblock.h"

/*
 * reiserfs_fs.h is too sick to include directly.
 * Use a cleaned up version.
 */
#include "reiserfs_fs.h"

#include "fstype.h"

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(x[0]))

#define BLOCK_SIZE 1024

static int gzip_image(const unsigned char *buf, unsigned long long *bytes)
{
	if (buf[0] == 037 && (buf[1] == 0213 || buf[1] == 0236)) {
		*bytes = 0;	/* Can only be determined by processing the whole file */
		return 1;
	}
	return 0;
}

static int cramfs_image(const unsigned char *buf, unsigned long long *bytes)
{
	const struct cramfs_super *sb =
		(const struct cramfs_super *)buf;

	if (sb->magic == CRAMFS_MAGIC) {
		if (sb->flags & CRAMFS_FLAG_FSID_VERSION_2)
			*bytes = (unsigned long long)sb->fsid.blocks << 10;
		else
			*bytes = 0;
		return 1;
	}
	return 0;
}

static int romfs_image(const unsigned char *buf, unsigned long long *bytes)
{
        const struct romfs_super_block *sb =
		(const struct romfs_super_block *)buf;

	if (sb->word0 == ROMSB_WORD0 && sb->word1 == ROMSB_WORD1) {
		*bytes = __be32_to_cpu(sb->size);
		return 1;
	}
	return 0;
}

static int minix_image(const unsigned char *buf, unsigned long long *bytes)
{
	const struct minix_super_block *sb =
		(const struct minix_super_block *)buf;

	if (sb->s_magic == MINIX_SUPER_MAGIC ||
	    sb->s_magic == MINIX_SUPER_MAGIC2) {
		*bytes = (unsigned long long)sb->s_nzones 
			<< (sb->s_log_zone_size+10);
		return 1;
	}
	return 0;
}

static int ext3_image(const unsigned char *buf, unsigned long long *bytes)
{
	const struct ext3_super_block *sb =
		(const struct ext3_super_block *)buf;

	if (sb->s_magic == __cpu_to_le16(EXT2_SUPER_MAGIC) &&
	    sb->s_feature_compat & __cpu_to_le32(EXT3_FEATURE_COMPAT_HAS_JOURNAL)) {
		*bytes = (unsigned long long)__le32_to_cpu(sb->s_blocks_count)
			<< (10+sb->s_log_block_size);
		return 1;
	}
	return 0;
}

static int ext2_image(const unsigned char *buf, unsigned long long *bytes)
{
	const struct ext2_super_block *sb =
		(const struct ext2_super_block *)buf;

	if (sb->s_magic == __cpu_to_le16(EXT2_SUPER_MAGIC)) {
		*bytes = (unsigned long long)__le32_to_cpu(sb->s_blocks_count)
			<< (10+sb->s_log_block_size);
		return 1;
	}
	return 0;
}

static int reiserfs_image(const unsigned char *buf, unsigned long long *bytes)
{
	const struct reiserfs_super_block *sb =
		(const struct reiserfs_super_block *)buf;

	if (memcmp(REISERFS_MAGIC(sb), REISERFS_SUPER_MAGIC_STRING,
		   sizeof(REISERFS_SUPER_MAGIC_STRING) - 1) == 0 ||
	    memcmp(REISERFS_MAGIC(sb), REISER2FS_SUPER_MAGIC_STRING,
		   sizeof(REISER2FS_SUPER_MAGIC_STRING) - 1) == 0 ||
	    memcmp(REISERFS_MAGIC(sb), REISER2FS_JR_SUPER_MAGIC_STRING,
		   sizeof(REISER2FS_JR_SUPER_MAGIC_STRING) - 1) == 0) {
		*bytes = (unsigned long long)REISERFS_BLOCK_COUNT(sb) *
			REISERFS_BLOCKSIZE(sb);
		return 1;
	}
	return 0;
}

static int xfs_image(const unsigned char *buf, unsigned long long *bytes)
{
	const struct xfs_sb *sb =
		(const struct xfs_sb *)buf;

	if (__be32_to_cpu(sb->sb_magicnum) == XFS_SB_MAGIC) {
		*bytes = __be64_to_cpu(sb->sb_dblocks) *
			__be32_to_cpu(sb->sb_blocksize);
		return 1;
	}
	return 0;
}

static int jfs_image(const unsigned char *buf, unsigned long long *bytes)
{
	const struct jfs_superblock *sb =
		(const struct jfs_superblock *)buf;

	if (!memcmp(sb->s_magic, JFS_MAGIC, 4)) {
		*bytes = __le32_to_cpu(sb->s_size);
		return 1;
	}
	return 0;
}

struct imagetype {
	off_t		block;
	const char	name[12];
	int		(*identify)(const unsigned char *, unsigned long long *);
};

static struct imagetype images[] = {
	{ 0,	"gzip",		gzip_image	},
	{ 0,	"cramfs",	cramfs_image	},
	{ 0,	"romfs",	romfs_image	},
	{ 0,	"xfs",		xfs_image	},
	{ 1,	"minix",	minix_image	},
	{ 1,	"ext3",		ext3_image	},
	{ 1,	"ext2",		ext2_image	},
	{ 8,	"reiserfs",	reiserfs_image	},
	{ 64,	"reiserfs",	reiserfs_image	},
	{ 32,	"jfs",		jfs_image	}
};

int identify_fs(int fd, const char **fstype, unsigned long long *bytes)
{
	unsigned char buf[BLOCK_SIZE];
	off_t cur_block = (off_t)-1;
	int i;
	int ret;

	*fstype = NULL;
	*bytes = 0;

	for (i = 0; i < ARRAY_SIZE(images); i++) {
		if (cur_block != images[i].block) {
			/*
			 * Read block.
			 */
			cur_block = images[i].block;
			ret = pread(fd, buf, BLOCK_SIZE, cur_block*BLOCK_SIZE);
			if (ret != BLOCK_SIZE)
				return -1; /* error */
		}

		if (images[i].identify(buf, bytes)) {
			*fstype = images[i].name;
			return 0;
		}
	}

	return 1;		/* Unknown filesystem */
}
