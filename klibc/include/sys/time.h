/*
 * sys/time.h
 */

#ifndef _SYS_TIME_H
#define _SYS_TIME_H

#include <extern.h>
#include <linux/time.h>

__extern int gettimeofday(struct timeval *, struct timezone *);
__extern int settimeofday(const struct timeval *, const struct timezone *);

#endif /* _SYS_TIME_H */
