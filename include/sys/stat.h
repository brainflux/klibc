/*
 * sys/stat.h
 */

#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <extern.h>
#include <asm/stat.h>
#include <linux/stat.h>

__extern int stat(const char *, struct stat *);
__extern int fstat(int, struct stat *);
__extern int lstat(const char *, struct stat *);

#endif /* _SYS_STAT_H */
