PROG=	sh
SRCS=	builtins.c cd.c dirent.c bltin/echo.c error.c eval.c exec.c expand.c \
	input.c jobs.c main.c memalloc.c miscbltin.c \
	mystring.c nodes.c options.c parser.c redir.c show.c \
	syntax.c trap.c output.c var.c bltin/test.c

OBJ1 =	init.o
OBJ2 =	builtins.o cd.o dirent.o bltin/echo.o error.o eval.o exec.o expand.o \
	input.o jobs.o main.o memalloc.o miscbltin.o \
	mystring.o nodes.o options.o parser.o redir.o show.o \
	syntax.o trap.o output.o var.o bltin/test.o

OBJS =	$(OBJ1) $(OBJ2)

KLIBSRC = ../klibc
include ../MCONFIG

MAKEDEPS = -Wp,-MD,.$(subst /,-,$*).d
CFLAGS   = $(MAKEDEPS) $(OPTFLAGS) $(REQFLAGS) -DSHELL
LIBS     = $(KLIBC) $(LIBGCC)
STRIP    = $(CROSS)strip -R .comment -R .note

HOST_CFLAGS  = -g -I. -DSHELL

CLEANFILES =\
	builtins.c builtins.h init.c mkinit mknodes \
	nodes.c nodes.h token.def

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all:	$(PROG)

$(PROG): $(OBJS) $(LIBS)
	$(LD) $(LDFLAGS) -o $(PROG) $(CRT0) $(OBJS) $(LIBS)
	$(STRIP) $(PROG)

$(CRT0) $(LIBS):
	@echo '*** error: $@ not up to date' || exit 1

install: all
	install sh /bin/ash
	install -m 644 sh.1 /usr/man/man1/ash.1

parser.o: token.def
token.def: mktokens
	sh ./mktokens

builtins.h: mkbuiltins builtins.in
	sh ./mkbuiltins

builtins.c: builtins.h
	: Generated as side effect

init.c: mkinit $(SRCS)
	./mkinit '$(HOST_CC) -c $(HOST_CFLAGS) init.c' $(SRCS)
	touch init.c

mkinit: mkinit.c
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) mkinit.c -o $@ $(HOST_LIBS)

nodes.h: mknodes nodetypes nodes.c.pat
	./mknodes nodetypes nodes.c.pat

nodes.c: nodes.h
	: Generated as side effect

mknodes: mknodes.c
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) mknodes.c -o $@ $(HOST_LIBS)

clean:
	rm -f core $(CLEANFILES) $(PROG) $(OBJS) .*.d

spotless: clean
	find . -name \*~ -not -type d -print0 | xargs -0rt rm -f

ifneq ($(wildcard .*.d),)
include $(wildcard .*.d)
endif
