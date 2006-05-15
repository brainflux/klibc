/*
 * chroot.c, by rmk
 */
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s newroot command...\n", argv[0]);
		return 1;
	}

	if (chroot(argv[1]) == -1) {
		perror("chroot");
		return 1;
	}

	if (execve(argv[2], argv + 2, envp) == -1) {
		perror("execve");
		return 1;
	}

	return 0;
}
