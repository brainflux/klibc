
#include <setjmp.h>

int __sigjmp_save (sigjmp_buf env, int savemask __attribute__ ((unused)))
{
  env[0].__mask_was_saved = 0;
  return 0;
}
