#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	unsigned long s;
	char *p;

	if (argc != 2)
		goto err;

	s = strtoul(argv[1], &p, 10);
	if ( *p )
		goto err;

	sleep(s);

	return 0;

err:
	fprintf(stderr, "Usage: %s seconds\n", argv[0]);
	return 1; 
}
