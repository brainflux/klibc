/*
 * __main.c
 *
 * Some architectures only have a minimal crt0.S stub.  This completes
 * the initialization code.
 */

#include <stdlib.h>
#include <unistd.h>

extern int main(int argc, char * const *argv, char * const *envp);

void __main(int argc, char **argv, char **envp)
{
  environ = envp;
  exit(main(argc, argv, envp));
}
