/*
 * unistd.h
 */

#ifndef _UNISTD_H
#define _UNISTD_H

#include <extern.h>
#include <stddef.h>
#include <sys/types.h>

/* Forward declarations of types */
struct timeval;

__extern char **environ;
__extern __noreturn _exit(int);

__extern pid_t fork(void);
__extern pid_t vfork(void);
__extern pid_t getpid(void);
__extern int setpgid(pid_t, pid_t);
__extern pid_t getppid(void);
__extern pid_t getpgrp(void);
__extern int setpgrp(void);
__extern pid_t setsid(void);
__extern int execve(const char *, const char * const *, char * const *);
__extern int execl(const char *, const char *, ...);
__extern int execv(const char *, const char * const *);

__extern int access(const char *, int);
__extern int link(const char *, const char *);
__extern int unlink(const char *);
__extern int chdir(const char *);
__extern int mknod(const char *, mode_t, dev_t);
__extern int chmod(const char *, mode_t);
__extern int mkdir(const char *, mode_t);
__extern int rmdir(const char *);
__extern int pipe(int *);
__extern int chroot(const char *);
__extern int symlink(const char *, const char *);
__extern int readlink(const char *, char *, size_t);

__extern int sync(void);

__extern ssize_t read(int, void *, size_t);
__extern ssize_t write(int, const void *, size_t);
#ifndef __IN_SYS_COMMON
__extern int open(const char *, int, ...);
#endif
__extern int close(int);
__extern off_t lseek(int, off_t, int);
__extern int dup(int);
__extern int dup2(int, int);
__extern int fcntl(int, int, long);
__extern int ioctl(int, int, void *);
__extern int flock(int, int);
__extern int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
__extern int fsync(int);

__extern int pause(void);
__extern unsigned int alarm(unsigned int);
__extern unsigned int sleep(unsigned int);
__extern void usleep(unsigned long);

__extern int gethostname(char *, size_t);
__extern int sethostname(const char *, size_t);
__extern int getdomainname(char *, size_t);
__extern int setdomainname(const char *, size_t);

#endif /* _UNISTD_H */
