/*
 * unistd.h
 */

#ifndef _UNISTD_H
#define _UNISTD_H

#include <extern.h>
#include <stddef.h>
#include <sys/types.h>

__extern char **environ;
__extern __noreturn _exit(int);

__extern pid_t getpid(void);
__extern pid_t getppid(void);

__extern pid_t fork(void);
__extern pid_t vfork(void);

__extern int execve(const char *, const char * const *, char * const *);
__extern int execl(const char *, const char *, ...);
__extern int execv(const char *, const char * const *);

__extern ssize_t read(int, void *, size_t);
__extern ssize_t write(int, const void *, size_t);

#endif /* _UNISTD_H */
