/*
 * compat.h
 *
 * Compability functions for Linux / Klibc
 */

#ifndef _LINUX_COMPAT_H
#define _LINUX_COMPAT_H

#define MAXPATHLEN PATH_MAX

#ifdef __KLIBC__
#define _PATH_TMP "/tmp/"
#endif /* __KLIBC__ */

#ifndef __KLIBC__
static inline size_t strlcpy ( char *   	  d,
			       const char *  	  s,
			       size_t  	  bufsize )  	
{
    size_t len = strlen(s);
    size_t ret = len;

    if (bufsize <= 0) return 0;
    if (len >= bufsize) len = bufsize-1;
    memcpy(d, s, len);
    d[len] = 0;
    return ret;
}
#endif

#endif
