/*
 * llseek.c
 *
 * On 32-bit platforms, we need llseek() as well as lseek() to be
 * able to handle large disks.  llseek() isn't just a normal syscall
 * which takes a 64-bit argument; it needs to return a 64-bit value
 * and so takes an extra pointer.
 */

#include <unistd.h>
#include <sys/syscall.h>

#if BITSIZE == 32

extern int __llseek(int fd, unsigned long hi, unsigned long lo, loff_t *res, int whence);

loff_t llseek(int fd, loff_t offset, int whence)
{
  loff_t result;
  int rv;

  rv = __llseek(fd, (unsigned long)(offset >> 32),
		(unsigned long)offset, &result, whence);
  
  return rv ? (loff_t)-1 : result;
}

#else

loff_t llseek(int fd, loff_t offset, int whence)
{
  return lseek(fd, offset, whence);
}

#endif

