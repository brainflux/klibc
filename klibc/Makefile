CC	= gcc
CFLAGS  = -g -O2 -march=i386 -mcpu=i686
LDFLAGS =
AR      = ar
RANLIB  = ranlib

TESTS   = testvsnp
LIBOBJS = vsnprintf.o snprintf.o vsscanf.o
LIB     = libmyc.a

all: $(TESTS)

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

testvsnp: testvsnp.o $(LIB)
	$(CC) $(LDFLAGS) -o $@ testvsnp.o $(LIB)

$(LIB): $(LIBOBJS)
	rm -f $(LIB)
	$(AR) cq $(LIB) $(LIBOBJS)
	$(RANLIB) $(LIB)

clean:
	rm -f $(TESTS) *.o

vsnprintf.o: vsnprintf.c

snprintf.o: snprintf.c
