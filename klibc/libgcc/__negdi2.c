#include <stdint.h>
#include <stddef.h>

struct DWstruct {int32_t low, high;};

typedef union
{
  struct DWstruct s;
  int64_t ll;
} DWunion;


__negdi2 (int64_t u)
{
  DWunion w;
  DWunion uu;

  uu.ll = u;

  w.s.low = -uu.s.low;
  w.s.high = -uu.s.high - ((uint32_t) w.s.low > 0);

  return w.ll;
}
