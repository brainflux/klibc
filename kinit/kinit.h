/*
 * kinit/kinit.h
 */

extern void dump_args(int argc, char *argv[]);
extern int do_nfsroot(int argc, char *argv[]);

#undef INI_DEBUG

#undef DEBUG
#ifdef INI_DEBUG
#define DEBUG(x) printf x
#else
#define DEBUG(x) do { } while (0)
#endif
