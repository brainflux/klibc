/*
 * getpwnam.c
 *
 * Dummy getpwnam() to support udev
 */

#include "userdb.h"


struct passwd *getpwnam(const char *name)
{
	if (!strcmp(name, "root"))
		return (struct passwd *)&__root_user;

	errno = ENOENT;
	return NULL;
}
