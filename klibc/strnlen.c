/*
 * strnlen()
 */

#include <string.h>

size_t strnlen(const char *s, size_t maxlen)
{
  const char *ss = s;
  while(*ss && (maxlen > 0)) {
	ss++;
	maxlen--;
  }
  return ss-s;
}

