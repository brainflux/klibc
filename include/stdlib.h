/*
 * stdlib.h
 */

#ifndef _STDLIB_H
#define _STDLIB_H

#include <extern.h>
#include <stddef.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

static __inline__ __noreturn _Exit(int __n) {
  __extern __noreturn _exit(int);
  _exit(__n);
}
__extern __noreturn abort(void);
static __inline__ int abs(int __n) {
  return (__n < 0) ? -__n : __n;
}
__extern int atexit(void (*)(void));
__extern int on_exit(void (*)(int, void *), void *);
__extern int atoi(const char *);
__extern long atol(const char *);
__extern long long atoll(const char *);
__extern __noreturn exit(int);
__extern void free(void *);
static __inline__ long labs(long __n) {
  return (__n < 0L) ? -__n : __n;
}

static __inline__ long llabs(long __n) {
  return (__n < 0LL) ? -__n : __n;
}
__extern void *malloc(size_t);
__extern void *realloc(void *, size_t);
__extern long strtol(const char *, char **, int);
__extern long long strtoll(const char *, char **, int);
__extern unsigned long strtoul(const char *, char **, int);
__extern unsigned long long strtoull(const char *, char **, int);

__extern char *getenv(const char *);
__extern int putenv(const char *);
__extern int setenv(const char *, const char *, int);
__extern int unsetenv(const char *);

#endif /* _STDLIB_H */
