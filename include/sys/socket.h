/*
 * sys/socket.h
 */

#ifndef _SYS_SOCKET_H
#define _SYS_SOCKET_H

#include <extern.h>
#include <linux/socket.h>

typedef int socklen_t;

__extern int socket(int, int, int);
__extern int accept(int, struct sockaddr *, socklen_t *);

#endif /* _SYS_SOCKET_H */
