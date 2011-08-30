/*
 * fclose.c
 */

#include "stdioint.h"

int fclose(FILE *f)
{
	int rv;

	fflush(f);

	rv = close(f->fd);

	/* Remove from linked list */
	f->next->prev = f->prev;
	f->prev->next = f->next;

	free(f->buf);
	free(f);

	return rv;
}
