/*
 * sys/vfs.h
 */

#ifndef _SYS_VFS_H
#define _SYS_VFS_H

#include <stdint.h>
#include <klibc/extern.h>
#include <sys/types.h>

/* struct statfs64 -- there seems to be two standards -
   one for 32 and one for 64 bits, and they're incompatible... */

#if BITSIZE == 32 || defined(__s390__)

struct statfs {
        uint32_t f_type;
        uint32_t f_bsize;
        uint64_t f_blocks;
        uint64_t f_bfree;
        uint64_t f_bavail;
        uint64_t f_files;
        uint64_t f_ffree;
        __kernel_fsid_t f_fsid;
        uint32_t f_namelen;
        uint32_t f_frsize;
        uint32_t f_spare[5];
};

#else /* BITSIZE == 64 */

struct statfs {
        uint64_t f_type;
        uint64_t f_bsize;
        uint64_t f_blocks;
        uint64_t f_bfree;
        uint64_t f_bavail;
        uint64_t f_files;
        uint64_t f_ffree;
        __kernel_fsid_t f_fsid;
        uint64_t f_namelen;
        uint64_t f_frsize;
        uint64_t f_spare[5];
};

#endif /* BITSIZE */

__extern int statfs(const char *, struct statfs *);
__extern int fstatfs(int, struct statfs *);

#endif /* _SYS_VFS_H */
