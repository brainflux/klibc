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

static const char *progname = "kinit";
int mnt_procfs;
int mnt_sysfs;

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
	int i, a = 1;

	argv[0] = (char *) "IP-Config";

	DEBUG(("Running ipconfig\n"));

#ifdef INI_DEBUG
	argv[a++] = (char *) "-n";
#endif

	for (i = 1; i < argc; i++) {
		if (strncmp(argv[i], "ip=", 3) == 0 ||
		    strncmp(argv[i], "nfsaddrs=", 9) == 0) {
			argv[a++] = argv[i];
		}
	}

	if (a > 1) {
		argv[a] = NULL;
		dump_args(a, argv);
		return ipconfig_main(a, argv);
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

static void save_cmdline(char **saved, int cmdc, char *cmdv[])
{
	int i;
	int len;
	char *x;

	for (i = len = 0; i < cmdc; i++)
		len += strlen(cmdv[i]) + 1;

	if ((x = *saved = malloc(len)) == NULL) {
		perror("malloc");
		exit(1);
	}

	for (i = 0; i < cmdc; i++) {
		strcpy(x, cmdv[i]);
		x += strlen(x) + 1;
	}
}

static void restore_cmdline(char *saved, int cmdc, char *cmdv[])
{
	int i;

	optind = 1;

	for (i = 0; i < cmdc; i++) {
		cmdv[i] = saved;
		saved += strlen(saved) + 1;
	}
	cmdv[cmdc] = NULL;
}

/* Was this argument passed? */
char *get_arg(int argc, char *argv[], const char *name)
{
	int len = strlen(name);
	char *ret = NULL;
	int i;

	for (i = 1; i < argc; i++) {
		if (argv[i] && strncmp(argv[i], name, len) == 0 &&
		    (argv[i][len] == '\0')) {
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
		fprintf(stderr, "NFS-Root: '%s' not a directory\n", path);
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	char **cmdv, *saved;
	char *kinit = NULL;
	char buf[1024];
	char *cmdline;
	int ret = 0;
	int cmdc;
	int fd;
	int i;

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

	save_cmdline(&saved, cmdc, cmdv);

	do_ipconfig(cmdc, cmdv);

	restore_cmdline(saved, cmdc, cmdv);
	check_path("/root");
	check_path("/old_root");
	do_mounts(cmdc, cmdv);

#ifndef INI_DEBUG
	if (pivot_root(".", "old_root") == -1) {
		perror("pivot_root");
		ret = 2;
		goto bail;
	}

	if (mnt_procfs == 1)
		umount2("/proc", 0);

	if (mnt_sysfs == 1)
		umount2("/sys", 0);

	for (i = 1; i < cmdc; i++) {
		if (strncmp(cmdv[i], "kinit=", 6) == 0) {
			kinit = cmdv[i] + 6;
		}
	}
	
	if (kinit) {
		char *s = strrchr(kinit, '/');
		if (s) {
			s++;
		}
		execl(kinit, s, NULL);
	}
	execl("/sbin/init", "init", NULL);
	execl("/etc/init", "init", NULL);
	execl("/bin/init", "init", NULL);
	execl("/bin/sh", "sh", NULL);

	fprintf(stderr, "%s: I give up - there's nothing to run.\n", progname);
	ret = 2;
	goto done;
#else
	printf("%s: imagine pivot_root and exec\n", progname);
#endif

 bail:
	if (mnt_procfs == 1)
		umount2("/proc", 0);

	if (mnt_sysfs == 1)
		umount2("/sys", 0);

 done:
	exit(ret);
}
