/*
 * sys/ioctl.h
 */

#ifndef _SYS_IOCTL_H
#define _SYS_IOCTL_H

#include <extern.h>
#include <linux/ioctl.h>

__extern int ioctl(int, int, void *);

#endif /* _SYS_IOCTL_H */
