/*
 * setresgid.c
 */

#include <unistd.h>
#include <sys/syscall.h>

#ifdef __NR_setresgid

_syscall3(int,setresgid,gid_t,a0,gid_t,a1,gid_t,a2);

#elif defined(__NR_setresgid32)

static inline _syscall3(int,setresgid32,gid_t,a0,gid_t,a1,gid_t,a2);

int setresgid(gid_t a0, gid_t a1, gid_t a2)
{
  uint32_t x0 = (a0 == (gid_t)-1) ? (uint32_t)-1 : a0;
  uint32_t x1 = (a1 == (gid_t)-1) ? (uint32_t)-1 : a1;
  uint32_t x2 = (a2 == (gid_t)-1) ? (uint32_t)-1 : a2;

  return setresgid32(x0,x1,x2);
}

#endif
