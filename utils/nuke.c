#ident "$Id: nuke.c,v 1.2 2004/06/08 02:58:27 hpa Exp $"
/* ----------------------------------------------------------------------- *
 *   
 *   Copyright 2004 H. Peter Anvin - All Rights Reserved
 *
 *   Permission is hereby granted, free of charge, to any person
 *   obtaining a copy of this software and associated documentation
 *   files (the "Software"), to deal in the Software without
 *   restriction, including without limitation the rights to use,
 *   copy, modify, merge, publish, distribute, sublicense, and/or
 *   sell copies of the Software, and to permit persons to whom
 *   the Software is furnished to do so, subject to the following
 *   conditions:
 *   
 *   The above copyright notice and this permission notice shall
 *   be included in all copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------- */

/*
 * nuke.c
 *
 * Simple program which does the same thing as rm -rf, except it takes
 * no options and can therefore not get confused by filenames starting
 * with -.  Similarly, an empty list of inputs is assumed to mean don't
 * do anything.
 */

#include <alloca.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

static const char *program;

static int nuke(const char *what);

static int nuke_dirent(int len, const char *dir, const char *name)
{
  int bytes = len+strlen(name)+2;
  char path[bytes];
  int xlen;

  xlen = snprintf(path, bytes, "%s/%s", dir, name);
  assert(xlen < bytes);

  return nuke(path);
}

static int nuke(const char *what)
{
  int rv;
  int err;

  rv = unlink(what);
  if ( rv < 0 && errno == EISDIR ) {
    /* It's a directory. */
    int len = strlen(what);
    DIR *dir;
    struct dirent *d;

    if ( !(dir = opendir(what)) ) {
      err = errno;
      if ( err == EACCES ) {
	/* Can't read it.  Might be empty and removable */
	if ( rmdir(what) == 0 )
	  return 0;
      }
      fprintf(stderr, "%s: %s: %s\n", program, what, strerror(err));
      return err;
    }

    err = 0;

    while ( (d = readdir(dir)) ) {
      /* Skip . and .. */
      if ( d->d_name[0] == '.' &&
	   (d->d_name[1] == '\0' ||
	    (d->d_name[1] == '.' && d->d_name[2] == '\0')) )
	continue;
      
      err = nuke_dirent(len, what, d->d_name);
      if ( err ) {
	closedir(dir);
	return err;
      }
    }

    closedir(dir);

    rv = rmdir(what);
  }

  if ( rv ) {
    err = errno;
    fprintf(stderr, "%s: %s: %s\n", program, what, strerror(err));
    return err;
  } else {
    return 0;
  }
}

int main(int argc, char *argv[])
{
  int i;
  int err = 0;

  program = argv[0];

  for ( i = 1 ; i < argc ; i++ ) {
    err = nuke(argv[i]);
    if ( err )
      break;
  }

  return !!err;
}
