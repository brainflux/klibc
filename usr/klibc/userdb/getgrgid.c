/*
 * getgrgid.c
 *
 * Dummy getgrgid() to support udev
 */

#include "userdb.h"

struct group *getgrgid(gid_t gid)
{
	if (!gid)
		return (struct group *)&__root_group;

	errno = ENOENT;
	return NULL;
}
