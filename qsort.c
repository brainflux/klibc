/*
 * qsort.c
 *
 * This is actually combsort.  It's an O(n log n) algorithm with
 * simplicity/small code size being its main virtue.
 */

#include <stddef.h>
#include <string.h>

static int newgap(int gap)
{
  gap = (gap*10)/13;
  if ( gap == 9 || gap == 10 )
    gap = 11;

  if ( gap < 1 )
    gap = 1;
  return gap;
}

void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
  int gap = nmemb;
  int swapped, i, j;
  void *p1, *p2;
  char tmp[size];

  do {
    gap = newgap(gap);
    swapped = 0;
    
    for ( i = 0, p1 = base ; i < nmemb-gap ; i++, (char *)p1 += size ) {
      j = i+gap;
      if ( compar(p1, p2 = (char *)base+j*size) > 0 ) {
	memcpy(tmp, p1, size);
	memcpy(p1, p2, size);
	memcpy(p2, tmp, size);
	swapped = 1;
      }
    }
  } while ( gap > 1 || swapped );
}

