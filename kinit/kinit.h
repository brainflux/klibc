/*
 * kinit/kinit.h
 */

extern void dump_args(int argc, char *argv[]);
extern int do_mounts(int argc, char *argv[]);
extern int mount_nfs_root(int argc, char *argv[], int flags);
extern char *get_arg(int argc, char *argv[], const char *name);

extern int mnt_procfs;
extern int mnt_sysfs;

#undef INI_DEBUG

#undef DEBUG
#ifdef INI_DEBUG
#define DEBUG(x) printf x
#else
#define DEBUG(x) do { } while (0)
#endif
