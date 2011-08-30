#include <stdio.h>

int ferror(FILE *f)
{
	return f->flags & _IO_FILE_FLAG_ERR;
}
