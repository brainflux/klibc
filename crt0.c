/* crt0.c - Run-time initialization */

#include <stdlib.h>
#include <stdint.h>

int errno;			/* It has to go somewhere... */
char **environ;

extern int main(int argc, char **argv, char **envp);

#if defined(__i386__)
register uintptr_t *params asm("%esp");
#elif defined(__x86_64__)
register uintptr_t *params asm("%rsp");
#elif defined(__sparc64__)
register uintptr_t sp asm("%sp");
#define BIAS 2047
#define params ((uintptr_t *)(sp+BIAS) + 16)
#elif defined(__sparc__) && !defined(__sparc64__)
register uintptr_t *sp asm("%sp");
#define params (sp+16)
#elif defined(__mips__) || defined(__mips64__)
register uintptr_t *params asm("$sp");
#elif defined(__powerpc__)
register uintptr_t *params asm("r9");
#elif defined(__hppa__)
# define STACK_GROWS_UP
register uintptr_t *params asm("%r25");
#elif defined(__s390__)
register uintptr_t *params asm("%r15");
#elif defined(__alpha__)
register uintptr_t *params asm("$sp");
#elif defined(__arm__)
register uintptr_t *params asm("sp");
#else
#error "Need crt0.c port for this architecture!"
#endif

void _start(void)
{
  /*
   * The argument block begins above the current stack frame, because we
   * have no return address.
   *
   * FIXME: this needs to be ported to all platforms...
   */

  int argc;
  char **argv;

  /* These seem to be standard for all the ELF ABIs... */
#ifdef STACK_GROWS_UP
  argc    = (int) *params;
  argv    = (char **)(params-1);
  environ = argv-argc-1;
#else
  argc    = (int) *params;
  argv    = (char **)(params+1);
  environ = argv+argc+1;
#endif

  exit(main(argc,argv,environ));
}
