#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kinit.h"
#include "ipconfig.h"

static const char *progname;

#define NARG 64

void dump_args(int argc, char *argv[])
{
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

	argv[0] = "IP-Config";

	DEBUG(("Running ipconfig\n"));

	argv[a++] = "-n";

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
	int vmax = *cmdc - 1;
	int v = 1, a;

	cmdv[0] = argv[0];

	while (i && *i && v < vmax) {
		if ((*i == ' ' || *i == '\t') && !was_space) {
			*i = '\0';
			was_space = 1;
		} else if (was_space) {
			cmdv[v++] = i;
			was_space = 0;
		}
		i++;
	}

	for (a = 1; a < argc && v < vmax; a++) {
		cmdv[v++] = argv[a];
	}

	cmdv[v] = NULL;

	return *cmdc = v;
}

static char *get_kernel_cmdline(char *buf, int len)
{
	FILE *fp;

	if ((fp = fopen("/proc/cmdline", "r")) == NULL) {
		fprintf(stderr, "%s: could not open kernel command line\n",
			progname);
	}
	else if (fgets(buf, len, fp) == NULL) {
		buf = NULL;
	} else {
		len = strlen(buf);
		if (buf[len - 1] == '\n') {
			buf[--len] = '\0';
		}
	}

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

int main(int argc, char *argv[])
{
	char *cmdv[NARG], *saved;
	int cmdc;
	char buf[1024];
	char *cmdline;

	cmdc = NARG;
	progname = argv[0];

	if ((cmdline = get_kernel_cmdline(buf, sizeof(buf))) == NULL)
		goto bail;

	if (split_cmdline(&cmdc, cmdv, cmdline, argc, argv) == 0)
		goto bail;

	save_cmdline(&saved, cmdc, cmdv);

	do_ipconfig(cmdc, cmdv);

	restore_cmdline(saved, cmdc, cmdv);
	do_nfsroot(cmdc, cmdv);

 bail:
	return 0;
}
