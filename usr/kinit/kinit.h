/*
 * kinit/kinit.h
 */

#include <stddef.h>

extern void dump_args(int argc, char *argv[]);
extern int do_mounts(int argc, char *argv[]);
extern int mount_nfs_root(int argc, char *argv[], int flags);
extern char *get_arg(int argc, char *argv[], const char *name);

extern int mnt_procfs;
extern int mnt_sysfs;

extern int init_argc;
extern char **init_argv;
extern const char *progname;

int open_cloexec(const char *path, int flags, mode_t mode);
int getintfile(const char *path, long *val);
ssize_t readfile(const char *path, char **pptr);
ssize_t freadfile(FILE *f, char **pptr);

#undef INI_DEBUG

#undef DEBUG
#ifdef INI_DEBUG
#define DEBUG(x) printf x
#else
#define DEBUG(x) do { } while (0)
#endif
