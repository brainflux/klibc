/*
 * free.c
 */

#include <stdlib.h>

#include "malloc.h"

void free(void *ptr)
{
  struct free_arena_header *ah, *pah, *nah;

  if ( !ptr )
    return;

  ah = (struct free_arena_header *)
    ((struct arena_header *)ptr - 1);
  
  pah = ah->a.prev;
  nah = ah->a.next;
  if ( pah->a.type == ARENA_TYPE_FREE &&
       (char *)pah+pah->a.size == (char *)ah ) {
    /* Coalesce into the previous block */
    pah->a.size += ah->a.size;
    pah->a.next = nah;
    nah->a.prev = pah;
    ah = pah;
    pah = ah->a.prev;
  }

  if ( nah->a.type == ARENA_TYPE_FREE &&
       (char *)ah+ah->a.size == (char *)nah ) {
    /* Coalesce into the subsequent block */
    ah->a.type = ARENA_TYPE_FREE;
    ah->a.size += nah->a.size;

    ah->next_free = nah->next_free;
    ah->prev_free = nah->prev_free;
    ah->next_free->prev_free = ah;
    ah->prev_free->next_free = ah;

    ah->a.next = nah->a.next;
    nah->a.next->a.prev = ah;
  }

  if ( ah->a.type != ARENA_TYPE_FREE ) {
    /* Neither.  Need to add this block to the free chain */
    ah->a.type = ARENA_TYPE_FREE;

    ah->next_free = __malloc_head.next_free;
    ah->prev_free = &__malloc_head;
    __malloc_head.next_free = ah;
    ah->next_free->prev_free = ah;
  }
}


