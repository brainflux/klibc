#	Makefile.linux

YACC = bison -y

COMMON_CFLAGS = -Wall $(OPTFLAGS) $(REQFLAGS)
COMMON_CPPFLAGS = \
	-include config.h \
	-DBSD=1 -DSMALL -DSHELL \
	-DGLOB_BROKEN -DFNMATCH_BROKEN -DIFS_BROKEN \
	-DJOBS=0

CFLAGS = $(COMMON_CFLAGS)
CPPFLAGS = $(COMMON_CPPFLAGS)
CFLAGS_FOR_BUILD = $(HOST_CFLAGS) $(COMMON_CFLAGS) 
CPPFLAGS_FOR_BUILD = $(COMMON_CPPFLAGS)
YFLAGS = -d

DEFS = -DHAVE_CONFIG_H
DEFAULT_INCLUDES = -I.
DEPDIR = .deps

COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(CPPFLAGS) $(CFLAGS)
COMPILE_FOR_BUILD = \
	$(HOST_CC) $(CPPFLAGS_FOR_BUILD) \
	$(CFLAGS_FOR_BUILD) 
YACCCOMPILE = $(YACC) $(YFLAGS)

PROG =	sh
SRCS =	alias.c arith_yylex.c cd.c error.c eval.c exec.c expand.c \
	histedit.c input.c jobs.c mail.c main.c memalloc.c miscbltin.c \
	mystring.c options.c parser.c redir.c show.c trap.c output.c \
	bltin/printf.c system.c bltin/test.c bltin/times.c var.c

OBJ1 =	builtins.o init.o nodes.o signames.o syntax.o
OBJ2 =	alias.o arith.o arith_yylex.o cd.o \
	error.o eval.o exec.o expand.o \
	histedit.o input.o jobs.o \
	mail.o main.o memalloc.o \
	miscbltin.o mystring.o options.o \
	parser.o redir.o show.o trap.o \
	output.o printf.o system.o \
	test.o times.o var.o

OBJS =	$(OBJ1) $(OBJ2)

SRCROOT  = ..
include ../MCONFIG

LIBS = $(KLIBC) $(LIBGCC)

HELPERS = mkinit mksyntax mknodes mksignames
BUILT_SOURCES = arith.h builtins.h nodes.h syntax.h token.h
CLEANFILES = \
	$(BUILT_SOURCES) $(patsubst %.o,%.c,$(OBJ1)) \
	arith.c $(HELPERS) builtins.def

all:	$(PROG) $(PROG).shared

$(OBJS): $(BUILT_SOURCES)

$(PROG): $(OBJS) $(LIBS) $(CRT0)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $(OBJS) $(LIBS)
	cp -f $@ $@.g
	$(STRIPCMD) $@

$(PROG).shared: $(OBJS) $(CRTSHARED) $(LIBSHARED) $(LIBGCC)
	$(LD) $(LDFLAGS) -o $(PROG).shared $(EMAIN) $(CRTSHARED) $(OBJS) -R $(LIBSHARED) $(LIBGCC)
	cp -f $@ $@.g
	$(STRIPCMD) $@

$(CRT0) $(LIBS):
	@echo '*** error: $@ not up to date' || exit 1

.c.o:
	if $(COMPILE) -MT $@ -MD -MP -MF ".$*.Td" -c -o $@ $<; \
	then mv -f ".$*.Td" ".$*.d"; else rm -f ".$*.Td"; exit 1; fi

printf.o: bltin/printf.c
	if $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -MT printf.o -MD -MP -MF ".printf.Td" -c -o printf.o `test -f 'bltin/printf.c' || echo '$(srcdir)/'`bltin/printf.c; \
	then mv -f ".printf.Td" ".printf.d"; else rm -f ".printf.Td"; exit 1; fi

test.o: bltin/test.c
	if $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -MT test.o -MD -MP -MF ".test.Td" -c -o test.o `test -f 'bltin/test.c' || echo '$(srcdir)/'`bltin/test.c; \
	then mv -f ".test.Td" ".test.d"; else rm -f ".test.Td"; exit 1; fi

times.o: bltin/times.c
	if $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -MT times.o -MD -MP -MF ".times.Td" -c -o times.o `test -f 'bltin/times.c' || echo '$(srcdir)/'`bltin/times.c; \
	then mv -f ".times.Td" ".times.d"; else rm -f ".times.Td"; exit 1; fi

.y.c:
	$(YACCCOMPILE) $<
	if test -f y.tab.h; then \
	  to=`echo "$*_H" | sed \
                -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/' \
                -e 's/[^ABCDEFGHIJKLMNOPQRSTUVWXYZ]/_/g'`; \
	  sed -e "/^#/!b" -e "s/Y_TAB_H/$$to/g" -e "s|y\.tab\.h|$*.h|" \
            y.tab.h >$*.ht; \
	  rm -f y.tab.h; \
	  if cmp -s $*.ht $*.h; then \
	    rm -f $*.ht ;\
	  else \
	    mv $*.ht $*.h; \
	  fi; \
	fi
	if test -f y.output; then \
	  mv y.output $*.output; \
	fi
	sed '/^#/ s|y\.tab\.c|$@|' y.tab.c >$@t && mv $@t $@
	rm -f y.tab.c

token.h: mktokens
	sh $^

builtins.def: builtins.def.in config.h
	$(COMPILE) -E -x c -o $@ $<

builtins.c builtins.h: mkbuiltins builtins.def
	sh $^

init.c: mkinit $(SRCS)
	./$^

nodes.c nodes.h: mknodes nodetypes nodes.c.pat
	./$^

syntax.c syntax.h: mksyntax
	./$^

signames.c: mksignames
	./$^

$(HELPERS): %: %.c
	$(COMPILE_FOR_BUILD) -o $@ $(CRT0) $< $(LIBS)

arith.h: arith.c
	@if test ! -f $@; then \
	  rm -f arith.c; \
	  $(MAKE) arith.c; \
	else :; fi

clean:
	rm -f core $(CLEANFILES) $(OBJS)
	rm -f $(PROG) $(PROG).g $(PROG).shared $(PROG).shared.g

spotless: clean
	find . \( -name \*~ -o -name \*.orig -o -name \*.rej -o -name \*.o \) -not -type d -print0 | xargs -0rt rm -f
	rm -f tags .*.d .*.Td

install: all
	$(INSTALL_EXEC) sh.shared $(INSTALLROOT)$(INSTALLDIR)/$(CROSS)bin/sh

ifneq ($(wildcard .*.d),)
include $(wildcard .*.d)
endif
