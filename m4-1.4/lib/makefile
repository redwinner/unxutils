# Generated automatically from Makefile.in by configure.
# Makefile for GNU m4 library.
# Copyright (C) 1992, 1993, 1994 Free Software Foundation, Inc.

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

PRODUCT = m4
VERSION = 1.4

SHELL = /bin/sh
srcdir = .

AR = lib
CC = cl
CFLAGS = -Ox -MD
CPPFLAGS = 
DEFS = -DHAVE_CONFIG_H
RANLIB = touch

COMPILE = $(CC) -c $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)

.SUFFIXES:
.SUFFIXES: .c .obj
.c.obj:
	$(COMPILE) $<

INCLUDES = -I.. -I$(srcdir)

HEADERS = getopt.h obstack.h regex.h
SOURCES = regex.c getopt.c getopt1.c error.c obstack.c xmalloc.c \
xstrdup.c alloca.c strtol.c
OBJECTS = regex.obj getopt.obj getopt1.obj error.obj obstack.obj xmalloc.obj \
xstrdup.obj  

DISTFILES = COPYING.LIB Makefile.in $(HEADERS) $(SOURCES) \
TAGS

all: libm4.a

libm4.lib: $(OBJECTS)
	rm -f m4.lib
	$(AR) /out:m4.lib $(OBJECTS)
	$(RANLIB) m4.lib

$(OBJECTS): ../config.h

install: all

uninstall:

tags: TAGS

TAGS: $(HEADERS) $(SOURCES)
	cd $(srcdir) && etags $(HEADERS) $(SOURCES)

mostlyclean:
	rm -f *.obj

clean: mostlyclean
	rm -f libm4.a

distclean: clean
	rm -f Makefile

realclean: distclean
	rm -f TAGS

dist: $(DISTFILES)
	@echo "Copying distribution files"
	@for file in $(DISTFILES); do \
	  ln $(srcdir)/$$file ../$(PRODUCT)-$(VERSION)/lib 2> /dev/null \
	    || cp -p $(srcdir)/$$file ../$(PRODUCT)-$(VERSION)/lib; \
	done

Makefile: Makefile.in ../config.status
	cd .. && CONFIG_FILES=lib/$@ CONFIG_HEADERS= ./config.status

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
