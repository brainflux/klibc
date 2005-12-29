#ifndef _KLIBC_ARCHSTAT_H
#define _KLIBC_ARCHSTAT_H

/* No nsec fields?! */
struct stat {
	unsigned long st_dev;
	unsigned short __pad1;
	ino_t   st_ino;
	mode_t  st_mode;
	unsigned int st_nlink;
	uid_t   st_uid;
	gid_t   st_gid;
	unsigned long st_rdev;
	unsigned short __pad2;
	off_t   st_size;
	off_t   st_blksize;
	off_t   st_blocks;
	time_t  st_atime;
	unsigned long __unused1;
	time_t  st_mtime;
	unsigned long __unused2;
	time_t  st_ctime;
	unsigned long __unused3;
	unsigned long __unused4;
	unsigned long __unused5;
};

#endif
