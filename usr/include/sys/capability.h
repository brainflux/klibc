#ifndef _SYS_CAPABILITY_H
#define _SYS_CAPABILITY_H

#include <klibc/extern.h>
#include <linux/capability.h>

__extern int capget(cap_user_header_t hdrp, cap_user_data_t datap);
__extern int capset(cap_user_header_t hdrp, const cap_user_data_t datap);

#endif				/* _SYS_CAPABILITY_H */
