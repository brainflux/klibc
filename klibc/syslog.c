/*
 * syslog.c
 *
 * Issue syslog messages via the kernel printk queue.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <sys/klog.h>

/* The kernel limits the size to 1024 chars anyway... */
#define BUFLEN 1024

void syslog(int prio, const char *format, ...)
{
  va_list ap;
  char buf[BUFLEN];
  int rv;

  va_start(ap, format);
  buf[0] = '<';
  buf[1] = LOG_PRI(prio)+'0';
  buf[2] = '>';
  /* -4 to make room for priority and newline */
  rv = vsnprintf(buf+3, BUFLEN-4, format, ap);
  if ( rv > BUFLEN-5 ) rv = BUFLEN-5;
  va_end(ap);

  buf[rv+3] = '\n';
  buf[rv+4] = '\0';

  klogctl(KLOG_WRITE, buf, rv+5);
}

