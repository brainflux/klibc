#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int i;

  /* Test argc/argv */
  for ( i = 0 ; i < argc ; i++ ) {
    printf("argv[%2d] = %s\n", i, argv[i]);
  }

  printf("PATH = %s\n", getenv("PATH"));
  printf("HOME = %s\n", getenv("HOME"));
  printf("TERM = %s\n", getenv("TERM"));
  printf("USER = %s\n", getenv("USER"));

  return 0;
}
