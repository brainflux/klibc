/* libc/sys/linux/crt0.c - Run-time initialization */

/* FIXME: This should be rewritten in assembler and
          placed in a subdirectory specific to a platform.
          There should also be calls to run constructors. */

/* Written 2000 by Werner Almesberger */


#include <stdlib.h>
#include <stdint.h>

int errno;			/* It has to go somewhere... */
char **environ;

extern int main(int argc, char **argv, char **envp);

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

#if defined(__i386__)
  register uintptr_t *params asm("%esp");
#elif defined(__x86_64__)
  register uintptr_t *params asm("%rsp");
#elif defined(__sparc__)
  register uintptr_t *sp asm("%sp");
  uintptr_t *params = sp+16;	/* SPARC needs a window save area */
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
