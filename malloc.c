/*
 * malloc.c
 *
 * Very simple linked-list based malloc().
 */

#include <stdlib.h>
#include "malloc.h"

void *sbrk(size_t);

struct free_arena_header __malloc_head =
{
  {
    ARENA_TYPE_HEAD,
    0,
    &__malloc_head,
    &__malloc_head,
  },
  &__malloc_head,
  &__malloc_head
};

static void *__malloc_from_block(struct free_arena_header *fp, size_t size)
{
  size_t fsize;
  struct free_arena_header *nfp, *na;

  fsize = fp->a.size;
  
  if ( fsize >= size+2*sizeof(struct arena_header) ) {
    /* Bigger block than required -- split block */
    nfp = (struct free_arena_header *)((char *)fp + size);
    na = fp->a.next;

    nfp->a.type = ARENA_TYPE_FREE;
    nfp->a.size = fsize-size;
    fp->a.type  = ARENA_TYPE_USED;
    fp->a.size  = size;

    /* Insert into all-block chain */
    nfp->a.prev = fp;
    nfp->a.next = na;
    na->a.prev = nfp;
    fp->a.next = nfp;
    
    /* Replace current block on free chain */
    nfp->next_free = fp->next_free;
    nfp->prev_free = fp->prev_free;
    fp->next_free->prev_free = nfp;
    fp->prev_free->next_free = nfp;
  } else {
    /* Allocate the whole block */
    fp->a.type = ARENA_TYPE_USED;

    /* Remove from free chain */
    fp->next_free->prev_free = fp->prev_free;
    fp->prev_free->next_free = fp->next_free;
  }
  
  return (void *)(&fp->a + 1);
}

void *malloc(size_t size)
{
  struct free_arena_header *fp;
  struct free_arena_header *pah;
  size_t fsize;

  /* Add the obligatory arena header, and round up */
  size = (size+2*sizeof(struct arena_header)-1) & ARENA_SIZE_MASK;

  for ( fp = __malloc_head.next_free ;	fp->a.size ; fp = fp->next_free ) {
    if ( fp->a.size >= size ) {
      /* Found fit -- allocate out of this block */
      return __malloc_from_block(fp, size);
    }
  }

  /* Nothing found... need to request a block from the kernel */
  fsize = (size+MALLOC_CHUNK_MASK) & ~MALLOC_CHUNK_MASK; 
  fp = (struct free_arena_header *) sbrk(fsize);

  if ( fp == (struct free_arena_header *)-1 ) {
    return NULL;		/* Failed to get a block */
  }

  /* Insert the block into the management chains */
  fp->a.type = ARENA_TYPE_FREE;
  fp->a.size = fsize;
  fp->next_free = __malloc_head.next_free;
  fp->prev_free = &__malloc_head;
  __malloc_head.next_free = fp;
  fp->next_free->prev_free = fp;
  
  /* We need to insert this into the main block list in the proper
     place -- this list is required to be sorted.  Since we most likely
     get memory assignments in ascending order, search backwards for
     the proper place. */

  for ( pah = &__malloc_head ; pah->a.type != ARENA_TYPE_HEAD ;
	pah = pah->a.prev ) {
    if ( pah < fp )
      break;
  }

  /* Now pah points to the node that should be the predecessor of
     the new node */
  fp->a.next = pah->a.next;
  fp->a.prev = pah;
  pah->a.next  = fp;
  fp->a.next->a.prev = fp;

  /* Now we can allocate from this block */
  return __malloc_from_block(fp, size);
}

