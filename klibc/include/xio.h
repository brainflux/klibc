/*
 * xio.h
 */

#ifndef _XIO_H
#define _XIO_H

#include <unistd.h>

ssize_t __xwrite(int, const void *, size_t);
ssize_t __xread(int, void *, size_t);

#endif /* _XIO_H */
