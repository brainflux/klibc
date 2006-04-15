#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include "kinit.h"

/*
 * devname.c
 *
 * Single interface to print a dev_t; we might end up adding sysfs
 * lookup here at some point...
 */
const char *bdevname(dev_t dev)
{
	static char buf[16];
	snprintf(buf, sizeof buf, "(%u,%u)", major(dev), minor(dev));
	return buf;
}
