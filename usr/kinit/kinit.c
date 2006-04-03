#include <sys/mount.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include <limits.h>

#include "kinit.h"
#include "ipconfig.h"
#include "run-init.h"

const char *progname = "kinit";
int mnt_procfs;
int mnt_sysfs;

void __attribute__((noreturn)) die(const char *msg)
{
	fprintf(stderr, "%s: %s: %s\n", progname, msg, strerror(errno));
	exit(1);
}

void dump_args(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

#ifdef INI_DEBUG
	int i;

	printf("%s: argc == %d\n", argv[0], argc);

	for (i = 1; i < argc; i++) {
		printf("  argv[%d]: '%s'\n", i, argv[i]);
	}
#endif
}

static int do_ipconfig(int argc, char *argv[])
{
	int i, a = 0;
	char **args = alloca((argc+1) * sizeof(char *));

	if ( !args )
		return -1;

	args[a++] = (char *) "IP-Config";

	DEBUG(("Running ipconfig\n"));

#ifdef INI_DEBUG
	args[a++] = (char *) "-n";
#endif

	for (i = 1; i < argc; i++) {
		if (strncmp(argv[i], "ip=", 3) == 0 ||
		    strncmp(argv[i], "nfsaddrs=", 9) == 0) {
			args[a++] = argv[i];
		}
	}

	if (a > 1) {
		args[a] = NULL;
		dump_args(a, args);
		return ipconfig_main(a, args);
	}

	return 0;
}

static int split_cmdline(int *cmdc, char *cmdv[],
			  char *cmdline,
			  int argc, char *argv[])
{
	char was_space = 1;
	char *i = cmdline;
	int vmax = *cmdc;
	int v = 1, a;

	if ( cmdv )
		cmdv[0] = argv[0];

	while (i && *i && v < vmax) {
		if ((*i == ' ' || *i == '\t') && !was_space) {
			if ( cmdv )
				*i = '\0';
			was_space = 1;
		} else if (was_space) {
			if ( cmdv )
				cmdv[v] = i;
			v++;
			was_space = 0;
		}
		i++;
	}

	for (a = 1; a < argc && v < vmax; a++) {
		if ( cmdv )
			cmdv[v] = argv[a];
		v++;
	}

	if ( cmdv )
		cmdv[v] = NULL;

	return *cmdc = v;
}

static int mount_sys_fs(const char *check, const char *fsname,
			const char *fstype)
{
	struct stat st;

	if (stat(check, &st) == 0) {
		return 0;
	}

	mkdir(fsname, 0555);

	if (mount("none", fsname, fstype, 0, NULL) == -1) {
		fprintf(stderr, "%s: could not mount %s as %s\n",
			progname, fsname, fstype);
		return -1;
	}

	return 1;
}

static char *get_kernel_cmdline(char *buf, int len)
{
	FILE *fp;

	if ((fp = fopen("/proc/cmdline", "r")) == NULL) {
		fprintf(stderr, "%s: could not open kernel command line\n",
			progname);
			buf = NULL;
			goto bail;
	}

	if (fgets(buf, len, fp) == NULL) {
		buf = NULL;
		goto bail;
	}

	len = strlen(buf);

	if (buf[len - 1] == '\n') {
		buf[--len] = '\0';
	}

 bail:
	if (fp) {
		fclose(fp);
	}

	return buf;
}

/* Was this boolean argument passed? */
int get_flag(int argc, char *argv[], const char *name)
{
	char **p;
	for (p = argv+1; *p; p++) {
		if (!strcmp(*p, name))
			return 1;
	}
	return 0;
}

/* Was this parameter passed? */
char *get_arg(int argc, char *argv[], const char *name)
{
	int len = strlen(name);
	char *ret = NULL;
	int i;

	for (i = 1; i < argc; i++) {
		if (argv[i] && strncmp(argv[i], name, len) == 0 &&
		    (argv[i][len] != '\0')) {
			ret = argv[i] + len;
			break;
		}
	}

	return ret;
}

static void check_path(const char *path)
{
	struct stat st;

	if (stat(path, &st) == -1) {
		if (errno != ENOENT) {
			perror("stat");
			exit(1);
		}
		if (mkdir(path, 0755) == -1) {
			perror("mkdir");
			exit(1);
		}
	}
	else if (!S_ISDIR(st.st_mode)) {
		fprintf(stderr, "%s: '%s' not a directory\n", progname, path);
		exit(1);
	}
}

static const char *find_init(const char *root, const char *user)
{
	const char *init_paths[] = {
		"/sbin/init", "/bin/init", "/etc/init", "/bin/sh", NULL
	};
	const char **p;
	const char *path;

	if ( chdir(root) ) {
		perror("chdir");
		exit(1);
	}

	if (user)
		DEBUG(("Checking for init: %s\n", user));

	if (user && user[0] == '/' && !access(user+1, X_OK)) {
		path = user;
	} else {
		for (p = init_paths; *p; p++) {
			DEBUG(("Checking for init: %s\n", *p));
			if ( !access(*p+1, X_OK) )
				break;
		}
		path = *p;
	}
	chdir("/");
	return path;
}

/* This is the argc and argv we pass to init */
const char *init_path;
int init_argc;
char **init_argv;

extern ssize_t readfile(const char *, char **);

int main(int argc, char *argv[])
{
	char **cmdv;
	char buf[1024];
	char *cmdline;
	const char *errmsg;
	int ret = 0;
	int cmdc;
	int fd;
	struct timeval now;

	gettimeofday(&now, NULL);
	srand48(now.tv_usec ^ (now.tv_sec << 24));

	/* Default parameters for anything init-like we execute */
	init_argc = argc;
	init_argv = alloca((argc+1)*sizeof(char *));
	memcpy(init_argv, argv, (argc+1)*sizeof(char *));

	if ((fd = open("/dev/console", O_RDWR)) != -1) {
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);

		if (fd > STDERR_FILENO) {
			close(fd);
		}
	}

	if ((mnt_procfs = mount_sys_fs("/proc/cmdline", "/proc", "proc")) == -1) {
		ret = 1;
		goto bail;
	}

	ret = readfile("/proc/cmdline", &cmdline);

	if ((mnt_sysfs = mount_sys_fs("/sys/bus", "/sys", "sysfs")) == -1) {
		ret = 1;
		goto bail;
	}

	if ((cmdline = get_kernel_cmdline(buf, sizeof(buf))) == NULL) {
		ret = 1;
		goto bail;
	}

	cmdc = INT_MAX;
	cmdv = (char **) alloca(sizeof(char *) *
	       (split_cmdline(&cmdc, NULL, cmdline, argc, argv+1)));

	if (split_cmdline(&cmdc, cmdv, cmdline, argc, argv) == 0) {
		ret = 1;
		goto bail;
	}

	/* Resume from suspend-to-disk, if appropriate */
	/* If successful, does not return */
	do_resume(cmdc, cmdv);

	/* Initialize networking, if applicable */
	do_ipconfig(cmdc, cmdv);

	check_path("/root");
	do_mounts(cmdc, cmdv);

	if (mnt_procfs) {
		umount2("/proc", 0);
		mnt_procfs = 0;
	}

	if (mnt_sysfs) {
		umount2("/sys", 0);
		mnt_sysfs = 0;
	}

	init_path = find_init("/root", get_arg(cmdc, cmdv, "init="));
	if ( !init_path ) {
		fprintf(stderr, "%s: init not found!\n", progname);
		ret = 2;
		goto bail;
	}

	init_argv[0] = strrchr(init_path, '/')+1;

	errmsg = run_init("/root", "/dev/console", init_path, init_argv);

	/* If run_init returned, something went bad */
	fprintf(stderr, "%s: %s: %s\n", progname, errmsg, strerror(errno));
	ret = 2;
	goto bail;

 bail:
	if (mnt_procfs)
		umount2("/proc", 0);

	if (mnt_sysfs)
		umount2("/sys", 0);

	exit(ret);
}
