PROG=	sh
SRCS=	builtins.c cd.c dirent.c bltin/echo.c error.c eval.c exec.c expand.c \
	input.c jobs.c mail.c main.c memalloc.c miscbltin.c \
	mystring.c nodes.c options.c parser.c redir.c show.c \
	syntax.c trap.c output.c var.c bltin/test.c

OBJ1 =	init.o
OBJ2 =	builtins.o cd.o dirent.o bltin/echo.o error.o eval.o exec.o expand.o \
	input.o jobs.o mail.o main.o memalloc.o miscbltin.o \
	mystring.o nodes.o options.o parser.o redir.o show.o \
	syntax.o trap.o output.o var.o bltin/test.o

OBJS =	$(OBJ1) $(OBJ2)

CC     = gcc
LD     = ld ../klibc/crt0.o
CFLAGS = -Os -fomit-frame-pointer -march=i386 -mcpu=i686 -DSHELL \
	-nostdinc -iwithprefix include \
	-I. -I../klibc/arch/i386/include -I../klibc/include/bits32 \
	-I../klibc/include -I../klibc/linux/include
LDFLAGS = 
LIBS    = ../klibc/libc.a
STRIP   = strip -R .comment -R .note

HOST_CC      = gcc
HOST_CFLAGS  = -g -I. -DSHELL
HOST_LDFLAGS =
HOST_LIBS    =

CLEANFILES =\
	builtins.c builtins.h init.c mkinit mknodes mksyntax \
	nodes.c nodes.h syntax.c syntax.h token.def

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all:	$(OBJS)
	$(LD) $(LDFLAGS) -o $(PROG) $(OBJS) $(LIBS)
	$(STRIP) $(PROG)

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

syntax.h: mksyntax
	./mksyntax

syntax.c: syntax.h
	: Generated as side effect

mksyntax: mksyntax.c parser.h
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) mksyntax.c -o $@ $(HOST_LIBS)

clean:
	rm -f core $(CLEANFILES) $(PROG) $(OBJS)

