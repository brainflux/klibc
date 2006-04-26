#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include "kinit.h"

/*
 * devname.c
 *
 * Print the name of a block device.
 */

const char *bdevname(dev_t dev)
{
	static char buf[512];
	char sysnode[512-64];	/* Must be smaller than buf */
	DIR *sysblock;
	FILE *sysdev;
	struct dirent *de;
	unsigned long ma, mi;
	int rd;
	char *ep;
	char *name = "dev";	/* Prints e.g. dev(3,5) */

	if ((sysblock = opendir("/sys/block"))) {
		while ((de = readdir(sysblock))) {
			/* Assume if we see a dot-name in sysfs it's special */
			if (de->d_name[0] == '.')
				continue;

			/* openfd() is really the right thing to use here... */

			if (snprintf(sysnode, sizeof sysnode,
				     "/sys/block/%s/dev", de->d_name)
			    >= sizeof sysnode)
				continue; /* Weird! */

			if (!(sysdev = fopen(sysnode, "r")))
				continue;

			rd = fread(sysnode, 1, sizeof sysnode-1, sysdev);
			sysnode[rd] = '\0'; /* Just in case... */

			fclose(sysdev);

			ma = strtoul(sysnode, &ep, 10);
			if (ma != major(dev) || *ep != ':')
				continue;

			mi = strtoul(ep+1, &ep, 10);
			if (mi != minor(dev) || *ep != '\n')
				continue;

			/* Found it! */
			name = de->d_name;
			break;
		}
		closedir(sysblock);
	}

	snprintf(buf, sizeof buf, "%s(%d,%d)",
		 name, major(dev), minor(dev));
	return buf;
}
