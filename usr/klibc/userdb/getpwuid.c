/*
 * getpwuid.c
 *
 * Dummy getpwuid() to support udev
 */

#include "userdb.h"


struct passwd *getpwuid(uid_t uid)
{
	if (!uid)
		return (struct passwd *)&__root_user;

	errno = ENOENT;
	return NULL;
}
