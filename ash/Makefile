#	Makefile.linux

KLIBC_SH_HISTORY = false
KLIBC_SH_PRINTF  = false
KLIBC_SH_ALIAS	 = false
KLIBC_SH_MAIL	 = false
KLIBC_SH_JOBS	 = false

PROG=	sh
SRCS =	arith.c arith_lex.c \
	builtins.c cd.c echo.c error.c eval.c exec.c expand.c \
	input.c jobs.c main.c memalloc.c miscbltin.c \
	mystring.c nodes.c options.c parser.c redir.c \
	show.c syntax.c trap.c output.c var.c test.c

OBJ1 =	init.o
OBJ2 =	arith.o arith_lex.o \
	builtins.o cd.o echo.o error.o eval.o exec.o expand.o \
	input.o jobs.o main.o memalloc.o miscbltin.o \
	mystring.o nodes.o options.o parser.o redir.o \
	show.o syntax.o trap.o output.o var.o test.o

OBJS =	$(OBJ1) $(OBJ2)

SHELL_DEFS = -DSHELL

ifeq ($(strip $(KLIBC_SH_HISTORY)),true)
SHELL_DEFS += -DKLIBC_SH_HISTORY
SRCS += histedit.c
OBJS += histedit.o
endif
ifeq ($(strip $(KLIBC_SH_PRINTF)),true)
SHELL_DEFS += -DKLIBC_SH_PRINTF
SRCS += printf.c
OBJS += printf.o
endif
ifeq ($(strip $(KLIBC_SH_ALIAS)),true)
SHELL_DEFS += -DKLIBC_SH_ALIAS
SRCS += alias.c
OBJS += alias.o
endif
ifeq ($(strip $(KLIBC_SH_MAIL)),true)
SHELL_DEFS += -DKLIBC_SH_MAIL
SRCS += mail.c
OBJS += mail.o
endif
ifeq ($(strip $(KLIBC_SH_JOBS)),true)
SHELL_DEFS += -DJOBS=1
SRCS += kill.c
OBJS += kill.o
else
SHELL_DEFS += -DJOBS=0
endif


OBJ_NODES = cd.o eval.o exec.o expand.o jobs.o main.o options.o parser.o redir.o show.o trap.o var.o
OBJ_SYNTAX = eval.o exec.o expand.o input.o jobs.o mystring.o output.o parser.o syntax.o trap.o var.o
OBJ_BUILDINS = eval.o exec.o

# bison produces substantially smaller output than byacc
YACC =  bison -y
LEX  =  lex

SRCROOT  = ..
include ../MCONFIG

MAKEDEPS = -Wp,-MD,.$(subst /,-,$*).d
SRC_DEFS = 
CFLAGS   = $(MAKEDEPS) $(OPTFLAGS) $(REQFLAGS) $(SHELL_DEFS) $(SRC_DEFS) -I.
LIBS     = $(KLIBC) $(LIBGCC)

HOST_CFLAGS  = -g -O -I. -DSHELL

CLEANFILES =\
	builtins.c builtins.h init.c \
	nodes.c nodes.h signames.c token.h \
	arith.c arith.h arith_lex.c \
	printf.c test.c echo.c kill.c

all:	$(PROG) $(PROG).shared

$(OBJS): Makefile

$(PROG): $(OBJS) $(LIBS) $(CRT0)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $(OBJS) $(LIBS)
	cp -f $@ $@.g
	$(STRIP) $@

$(PROG).shared: $(OBJS) $(CRTSHARED) $(LIBSHARED) $(LIBGCC)
	$(LD) $(LDFLAGS) -o $(PROG).shared $(EMAIN) $(CRTSHARED) $(OBJS) -R $(LIBSHARED) $(LIBGCC)
	cp -f $@ $@.g
	$(STRIP) $@

$(CRT0) $(LIBS):
	@echo '*** error: $@ not up to date' || exit 1

parser.o: token.h

token.h: mktokens
	sh ./mktokens

$(OBJ_BUILDINS): builtins.h
builtins.def: Makefile
builtins.h builtins.c: mkbuiltins shell.h builtins.def
	sh ./mkbuiltins shell.h builtins.def . "$(CFLAGS)"

init.c: mkinit.sh $(SRCS)
	sh mkinit.sh $(SRCS)
	touch init.c

$(OBJ_NODES): nodes.h
$(OBJ_SYNTAX): syntax.h

nodes.c nodes.h: mknodes.sh nodetypes nodes.c.pat nodes
nodes:
	sh mknodes.sh nodetypes nodes.c.pat .

signames.c: mksignames
	./mksignames

echo.c: bltin/echo.c
	ln -sf $< $@

test.c: bltin/test.c
	ln -sf $< $@

kill.c: bltin/kill.c
	ln -sf $< $@

printf.c: bltin/printf.c
	ln -s $< $@

arith.c: arith.y
	$(YACC) $<
	mv y.tab.c $@

arith_lex.o: arith_lex.c arith.h

arith_yylex.o: arith_yylex.c arith.h

arith_lex.c: arith_lex.l
	$(LEX) $<
	mv lex.yy.c $@

arith.h: arith.c
	sed -n '/^#define ARITH/p' $< > $@

clean:
	rm -f core $(CLEANFILES) $(OBJS) .*.d *.g
	rm -f $(PROG) $(PROG).shared

spotless: clean
	find . \( -name \*~ -o -name \*.orig -o -name \*.rej -o -name \*.o \) -not -type d -print0 | xargs -0rt rm -f
	rm -f tags

ifneq ($(wildcard .*.d),)
include $(wildcard .*.d)
endif
