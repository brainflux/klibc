/*
 * vsscanf.c
 *
 * vsscanf(), from which the rest of the scanf()
 * family is built
 */

#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

enum flags {
  FL_SPLAT  = 0x01,		/* Drop the value, do not assign */
  FL_INV    = 0x02,		/* Character-set with inverse */
  FL_WIDTH  = 0x04,		/* Field width specified */
  FL_MINUS  = 0x08,		/* Negative number */
};

enum ranks {
  rank_char	= -2,
  rank_short	= -1,
  rank_int 	= 0,
  rank_long	= 1,
  rank_longlong	= 2,
  rank_ptr      = INT_MAX	/* Special value used for pointers */
};

#define MIN_RANK	rank_char
#define MAX_RANK	rank_longlong

#define INTMAX_RANK	rank_longlong
#define SIZE_T_RANK	rank_long
#define PTRDIFF_T_RANK	rank_long

static inline const char *
skipspace(const char *p)
{
  while ( isspace(*p) ) p++;
  return p;
}

int my_vsscanf(const char *buffer, size_t n, const char *format, va_list ap)
{
  const char *p = format;
  char ch;
  const char *q = buffer;
  const char *qq;
  uintmax_t val = 0;
  int rank = rank_int;		/* Default rank */
  unsigned int width = UINT_MAX;
  int base;
  size_t sz;
  enum flags flags = 0;
  enum {
    st_normal,			/* Ground state */
    st_flags,			/* Special flags */
    st_width,			/* Field width */
    st_modifiers		/* Length or conversion modifiers */
  } state = st_normal;
  char *sarg;			/* %s %c or %[ string argument */
  int slen;			/* String length */
  int bail = 0;			/* Set to true if hopeless */
  int sign, minus;
  int converted = 0;		/* Successful conversions */

  while ( (ch = *p++) && !bail ) {
    switch ( state ) {
    case st_normal:
      if ( ch == '%' ) {
	state = st_flags;
	flags = 0; rank = rank_int; width = UINT_MAX;
      } else if ( isspace(ch) ) {
	q = skipspace(q);
      } else {
	if ( *q == ch )
	  q++;
	else
	  bail = 1;		/* Match failure */
      }
      break;

    case st_flags:
      switch ( ch ) {
      case '*':
	flags |= FL_SPLAT;
	break;
      case '0' ... '9':
	width = (ch-'0');
	state = st_width;
	flags |= FL_WIDTH;
	break;
      default:
	state = st_modifiers;
	p--;			/* Process this character again */
	break;
      }
      break;

    case st_width:
      if ( ch >= '0' && ch <= '9' ) {
	width = width*10+(ch-'0');
      } else {
	state = st_modifiers;
	p--;			/* Process this character again */
      }
      break;

    case st_modifiers:
      switch ( ch ) {
	/* Length modifiers - nonterminal sequences */
      case 'h':
	rank--;			/* Shorter rank */
	break;
      case 'l':
	rank++;			/* Longer rank */
	break;
      case 'j':
	rank = INTMAX_RANK;
	break;
      case 'z':
	rank = SIZE_T_RANK;
	break;
      case 't':
	rank = PTRDIFF_T_RANK;
	break;
      case 'L':
      case 'q':
	rank = rank_longlong;	/* long double/long long */
	break;

      default:
	/* Output modifiers - terminal sequences */
	state = st_normal;	/* Next state will be normal */
	if ( rank < MIN_RANK )	/* Canonicalize rank */
	  rank = MIN_RANK;
	else if ( rank > MAX_RANK )
	  rank = MAX_RANK;

	switch ( ch ) {
	case 'P':		/* Upper case pointer */
	case 'p':		/* Pointer */
	  q = skipspace(q);
	  if ( !isdigit(*q) ) {
	    static const char *nullnames[] =
	    { "null", "nul", "nil", "(null)", "(nul)", "(nil)", 0 };
	    const char **np;

	    /* Check to see if it's a null pointer by name */
	    for ( np = nullnames ; *np ; np++ ) {
	      if ( !strncasecmp(q, *np, strlen(*np)) ) {
		val = (uintmax_t)((void *)NULL);
		goto set_integer;
	      }
	    }
	    /* Failure */
	    bail = 1;
	    break;
	  }
	  /* else */
	  rank = rank_ptr;
	  base = 0; sign = 0;
	  goto scan_int;

	case 'i':		/* Base-independent integer */
	  base = 0; sign = 1;
	  goto scan_int;

	case 'd':		/* Decimal integer */
	  base = 10; sign = 1;
	  goto scan_int;

	case 'o':		/* Octal integer */
	  base = 8; sign = 0;
	  goto scan_int;

	case 'u':		/* Unsigned decimal integer */
	  base = 10; sign = 0;
	  goto scan_int;
	  
	case 'x':		/* Hexadecimal integer */
	case 'X':
	  base = 16; sign = 0;
	  goto scan_int;

	case 'n':		/* Number of characters consumed */
	  val = (q-buffer);
	  goto set_integer_noinc;

	scan_int:
	  q = skipspace(q);
	  minus = 0;
	  if ( sign && width && *q == '-' ) {
	    q++; width--; minus = 1;
	  }
	  val = strntoumax(q, &qq, base, width);
	  if ( qq == q ) {
	    bail = 1;
	    break;
	  }
	  if (minus) {
	    val = (uintmax_t) -(intmax_t)val;
	  }
	  /* fall through */

	set_integer:
	  converted++;
	  /* fall through */

	set_integer_noinc:
	  if ( !(flags & FL_SPLAT) ) {
	    switch(rank) {
	    case rank_char:
	      *va_arg(ap, unsigned char *) = (unsigned char)val;
	      break;
	    case rank_short:
	      *va_arg(ap, unsigned short *) = (unsigned short)val;
	      break;
	    case rank_int:
	      *va_arg(ap, unsigned int *) = (unsigned int)val;
	      break;
	    case rank_long:
	      *va_arg(ap, unsigned long *) = (unsigned long)val;
	      break;
	    case rank_longlong:
	      *va_arg(ap, unsigned long long *) = (unsigned long long)val;
	      break;
	    case rank_ptr:
	      *va_arg(ap, void **) = (void *)(uintptr_t)val;
	      break;
	    }
	  }
	  break;
	  
	case 'c':               /* Character */
          width = (flags & FL_WIDTH) ? width : 1; /* Default width == 1 */
          sarg = va_arg(ap, char *);
          while ( width ) {
            if ( !*q ) {
              bail = 1;
              break;
            }
            *sarg++ = *q++;
            width--;
          }
          if ( !bail )
            converted++;
          break;

        case 's':               /* String */
	  /* do stuff */
	  break;
	  
	case '[':		/* Character range */
	  /* do stuff */
	  break;

	case '%':		/* %% sequence */
	  if ( *q == '%' )
	    q++;
	  else
	    bail = 1;
	  break;

	default:		/* Anything else, including % */
	  /* wtf? */
	  break;
	}
      }
    }
  }

  return converted;
}
