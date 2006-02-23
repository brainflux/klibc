/*
 * mmap.c
 */

#include <stdint.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <bitsize.h>

/*
 * Set in SYSCALLS whether or not we should use an unadorned mmap() system
 * call (typical on 64-bit architectures).
 */
#if (_BITSIZE == 32 && defined(__NR_mmap2)) || (_BITSIZE == 64 && !defined(__NR_mmap))

/*
 * Some architectures use a fixed value for MMAP2_SHIFT, other use the
 * current page size.  This, of course, isn't documented or even
 * #define'd anywhere.
 */
#if defined(__cris__) || defined(__m68k__) || defined(__mips__)
/* Use the current page size */
#else
# define MMAP2_SHIFT	12	/* Fixed by syscall definition */
#endif


/* This architecture uses mmap2(). The Linux mmap2() system call takes
   a page offset as the offset argument.  We need to make sure we have
   the proper conversion in place. */

extern void *__mmap2(void *, size_t, int, int, int, size_t);

void *mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset)
{
#ifdef MMAP2_SHIFT
  const int mmap2_shift = MMAP2_SHIFT;
#else
  extern unsigned int __page_shift;
  const int mmap2_shift = __page_shift;
#endif
  extern unsigned int __page_size;
  const unsigned int mmap2_mask = __page_size - 1;

  if ( offset & mmap2_mask ) {
    errno = EINVAL;
    return MAP_FAILED;
  }

  return __mmap2(start, length, prot, flags, fd, (size_t)offset >> mmap2_shift);
}

#endif


    
  
