#ifndef _SYS_PRCTL_H
#define _SYS_PRCTL_H

#include <linux/prctl.h>

/* glibc has this as a varadic function, so join the club... */
int prctl(int, ...);

#endif /* _SYS_PRCTL_H */
