#include <stdio.h>
#include <unistd.h>
#include <xio.h>

int main(void)
{
  const char hello[] = "Hello, World!\n";
  write(1, hello, sizeof hello-1);
  return 0;
}
