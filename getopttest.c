/*
 * getopttest.c
 *
 * Simple test for getopt, set the environment variable GETOPTTEST
 * to give the argument string to getopt()
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char * const *argv)
{
  const char *parser;
  int c;

  parser = getenv("GETOPTTEST");
  if ( !parser ) parser = "abzf:o:";

  do {
    c = getopt(argc, argv, parser);
    printf("c = \'%c\', optind = %d (%s), optarg = \"%s\", optopt = \'%c\'\n",
	   c, optind, argv[optind], optarg, optopt);
  } while ( c != -1 );
  
  return 0;
}

