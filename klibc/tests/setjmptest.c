/*
 * setjmptest.c
 */

#include <stdio.h>
#include <setjmp.h>

static jmp_buf buf;

int do_stuff(int v)
{
  printf("setjmp returned %d\n", v);
  longjmp(buf, v+1);
}

int main(void)
{
  int v;

  v = setjmp(buf);

  if ( v < 4 )
    do_stuff(v);

  return 0;
}
