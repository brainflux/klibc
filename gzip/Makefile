# Generated automatically from Makefile.in by configure.
# Makefile for gzip (GNU zip)    -*- Indented-Text -*-
# Copyright (C) 1992-1993 Jean-loup Gailly and the Free Software Foundation

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

# $Id: Makefile,v 1.5 2004/02/09 23:18:14 hpa Exp $

#### Start of system configuration section. ####

SRCROOT = ..
include ../MCONFIG
include MCONFIG
include ../MRULES

#### End of system configuration section. ####

MAKEDEPS = -Wp,-MD,.$(subst /,-,$*).d
CFLAGS   = $(MAKEDEPS) $(DEFS) $(OPTFLAGS) $(REQFLAGS) $(WARNFLAGS)

ZIPSRCS = zip.c deflate.c trees.c bits.c
UNZIPSRCS = unzip.c inflate.c

SRCS = gzip.c util.c $(UNZIPSRCS)
OBJS = $(SRCS:.c=.o)
HDRS = gzip.h tailor.h revision.h
LIBS = $(KLIBC) $(LIBGCC)

ifeq ($(ZIP),y)
DEFS += -DSUPPORT_ZIP
SRCS += $(ZIPSRCS)
endif

all:	gzip gunzip zcat gzip.stripped

# Actual build-related targets

gzip:	Makefile $(OBJS) $(CRT0) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $(OBJS) $(LIBS)

$(CRT0) $(LIBS):
	@echo '*** error: $@ not up to date' || exit 1

gunzip:	gzip
	rm -f $@
	ln $< $@

zcat: gzip
	rm -f $@
	ln $< $@

gzip.stripped: gzip
	$(STRIP) $< -o $@

clean:
	$(RM) -f core *.o gzip gunzip zcat gzip.stripped .*.d

spotless: clean
	rm -f *~

ifneq ($(wildcard .*.d),)
include $(wildcard .*.d)
endif
