#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

static sig_atomic_t counter = 0;

static void sig_handler(int signum)
{
  static char msg[] = "Signal handler\n";

  (void)signum;

  write(1, msg, sizeof msg - 1);
  counter++;
}

int main(int argc, char *argv[])
{
  struct sigaction act;
  pid_t f;
  
  memset(&act, 0x00, sizeof(struct sigaction));
  act.sa_handler = sig_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;
  sigaction(SIGINT, &act, NULL);
  sigaction(SIGTERM, &act, NULL);
  
  (void)argc;

  f = fork();

  if ( f < 0 ) {
    perror(argv[0]);
    exit(255);
  } else if ( f > 0 ) {
    sleep(5);
    if ( !counter ) {
      fprintf(stderr, "No signal received!\n");
      exit(1);
    }
    exit(0);
  } else {
    sleep(1);
    kill(getppid(), SIGINT);
    exit(0);
  }
}
