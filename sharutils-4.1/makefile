# Generated automatically from Makefile.in by configure.
# Makefile for GNU shar utilities.
# Copyright (C) 1994 Free Software Foundation, Inc.
# Francois Pinard <pinard@iro.umontreal.ca>, 1994.

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

PRODUCT = sharutils
VERSION = 4.1
PROGRAMS = shar unshar uudecode uuencode

SHELL = /bin/sh
srcdir = .

# These CFLAGS values, all pertaining to shar, are not auto-configured:
# -DDEBUG=1		use -$ on call for activating debugging output
# -DMSDOS=1		probably doesn't work but accounts for some old code
# -DNO_WALKTREE=1	inhibit recursive walk on directory parameters
# -DSTRNCMP_IS_FAST=1	if unefficient to compare first chars before strncmp

AR = lib
CC = cl
CFLAGS =  -Ox   -MD -DWIN32
CPPFLAGS = 
DEFS = -DHAVE_CONFIG_H
INSTALL = /usr/wbin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
LDFLAGS = 
LIBS = \\projects\\cpp\\downhill\\downhill.lib wsock32.lib setargv.obj binmode.obj # /link  /debug
MAKEINFO = makeinfo
RANLIB = touch
TEXI2DVI = texi2dvi

prefix = /usr/local
exec_prefix = ${prefix}
transform = s,x,x,
bindir = $(exec_prefix)/wbin
infodir = $(prefix)/info
man1dir = $(prefix)/man/man1
man1ext = .1
man5dir = $(prefix)/man/man5
man5ext = .5

COMPILE = $(CC) -c $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LINK = $(CC) $(LDFLAGS) -o $@

.SUFFIXES:
.SUFFIXES: .c .obj
.c.obj:
	$(COMPILE) $<

ANSI2KNR = 
O = .obj

.SUFFIXES: ._c ._obj
.c._c:
	./ansi2knr $< > $@
._c._obj:
	@echo $(COMPILE) $<
	@rm -f _$*.c
	@ln $< _$*.c && $(COMPILE) _$*.c && mv _$*.obj $@ && rm _$*.c
.c._obj:
	./ansi2knr $< > $*._c
	@echo $(COMPILE) $*._c
	@rm -f _$*.c
	@ln $*._c _$*.c && $(COMPILE) _$*.c && mv _$*.obj $@ && rm _$*.c

INCLUDES = -I. -I$(srcdir) -I/projects/cpp/downhill/include

HEADERS = system.h
SOURCES = encode.c shar.c unshar.c uudecode.c uuencode.c whoami.c
OBJECTS = encode$O shar$O unshar$O uudecode$O uuencode$O whoami$O
LIBHDRS = getopt.h pathmax.h
LIBSRCS = error.c getopt.c getopt1.c xgetcwd.c xmalloc.c \
xstrdup.c alloca.c basename.c memset.c mktime.c strftime.c
LIBOBJS = error.obj getopt.obj getopt1.obj xgetcwd.obj xmalloc.obj \
xstrdup.obj   basename.obj

DISTFILES = README NEWS TODO THANKS COPYING INSTALL ChangeLog \
README.OLD ChangeLog.OLD \
configure.in aclocal.m4 acconfig.h mkinstalldirs install-sh Makefile.in \
sharutils.texi texinfo.tex shar.1 unshar.1 uuencode.1 uuencode.5 \
ansi2knr.c ansi2knr.1 c-boxes.el \
$(HEADERS) $(SOURCES) $(LIBHDRS) $(LIBSRCS) testdata \
stamp-h.in config.h.in configure \
stamp-vti version.texi sharutils.info TAGS BACKLOG

all: sharutils.info $(PROGRAMS)

info: sharutils.info

sharutils.info: sharutils.texi version.texi
	cd $(srcdir) && $(MAKEINFO) --no-split sharutils.texi

dvi: sharutils.dvi

sharutils.dvi: sharutils.texi
	$(TEXI2DVI) $(srcdir)/sharutils.texi

version.texi: stamp-vti
stamp-vti: sharutils.texi configure.in
	echo "@set EDITION $(VERSION)" > version.tmp
	echo "@set UPDATED `date '+%B %Y'`" >> version.tmp
	echo "@set VERSION $(VERSION)" >> version.tmp
	if cmp -s version.tmp $(srcdir)/version.texi; then rm version.tmp; \
	else mv version.tmp $(srcdir)/version.texi; fi
	date > $(srcdir)/stamp-vti

shar: shar$O encode$O whoami$O shutl.lib
	$(LINK) shar$O encode$O whoami$O shutl.lib $(LIBS)

unshar: unshar$O shutl.lib
	$(LINK) unshar$O shutl.lib $(LIBS)

uudecode: uudecode$O shutl.lib
	$(LINK) uudecode$O shutl.lib $(LIBS)

uuencode: uuencode$O shutl.lib
	$(LINK) uuencode$O shutl.lib $(LIBS)

$(OBJECTS): $(ANSI2KNR) config.h system.h
shar$O unshar$O uudecode$O uuencode$O: getopt.h

ansi2knr: ansi2knr.obj
	$(LINK) ansi2knr.obj $(LIBS)

shutl.lib: $(LIBOBJS)
	rm -f shutl.lib
	$(AR) /out:shutl.lib $(LIBOBJS)

$(LIBOBJS): config.h
getopt.obj getopt1.o: getopt.h
xgetcwd.obj: pathmax.h

check: uuencode uudecode testdata
	rm -f test.bin test.tmp
	./uudecode $(srcdir)/testdata
	./uuencode test.bin test.bin > test.tmp
	cmp $(srcdir)/testdata test.tmp
	rm -f test.bin test.tmp
	@echo Check successful

install: all
	$(srcdir)/mkinstalldirs $(bindir) $(infodir)
	for name in $(PROGRAMS); do \
	  $(INSTALL_PROGRAM) $$name \
		$(bindir)/`echo $$name | sed '$(transform)'`; \
	done
	$(INSTALL_DATA) $(srcdir)/sharutils.info $(infodir)/sharutils.info

uninstall:
	for name in $(PROGRAMS); do
	  rm -f $(bindir)/`echo $$name | sed '$(transform)'`; \
	done
	rm -f $(infodir)/sharutils.info

# man page installation is kept separate from install.  Better think
# GNU and switch to Info.  The following is for irreductible fellows.

install-man:
	$(srcdir)/mkinstalldirs $(man1dir) $(man5dir)
	for name in shar unshar uuencode; do \
	  $(INSTALL_DATA) $(srcdir)/$$name.1 \
		$(man1dir)/`echo $$name | sed '$(transform)'`$(man1ext); \
	done
	rm -f $(man1dir)/`echo uudecode | sed '$(transform)'`$(man1ext)
	ln $(man1dir)/`echo uuencode | sed '$(transform)'`$(man1ext) \
		$(man1dir)/`echo uudecode | sed '$(transform)'`$(man1ext)
	$(INSTALL_DATA) $(srcdir)/`echo uuencode | sed '$(transform)'`.5 \
		$(man5dir)/`echo uuencode | sed '$(transform)'`$(man5ext)

uninstall-man:
	for name in shar unshar uudecode uuencode; do \
	  rm -f $(man1dir)/`echo $$name | sed '$(transform)'`$(man1ext); \
	done
	rm -f $(man5dir)/`echo uuencode | sed '$(transform)'`$(man5ext)

tags: TAGS

TAGS: $(HEADERS) $(SOURCES) $(LIBHDRS) $(LIBSRCS)
	cd $(srcdir) && etags $(HEADERS) $(SOURCES) $(LIBHDRS) $(LIBSRCS)

mostlyclean:
	rm -f *.aux *.cp *.cps *.dvi *.fn *.fns *.ky *.log *.pg *.toc *.tp *.vr
	rm -f *.a *.obj *._c *._o core core.*
	rm -f *.tmp test.bin test.tmp

clean: mostlyclean
	rm -f shar unshar uudecode uuencode

distclean: clean
	rm -f ansi2knr
	rm -f Makefile config.cache config.h config.log stamp-h config.status

realclean: distclean
	rm -f stamp-vti version.texi sharutils.info
	rm -f TAGS

dist: $(DISTFILES)
	rm -rf $(PRODUCT)-$(VERSION)
	mkdir $(PRODUCT)-$(VERSION)
	chmod 777 $(PRODUCT)-$(VERSION)
	@echo "Copying distribution files"
	@for file in $(DISTFILES); do \
	  ln $(srcdir)/$$file $(PRODUCT)-$(VERSION) 2> /dev/null \
	    || cp -p $(srcdir)/$$file $(PRODUCT)-$(VERSION); \
	done
	chmod -R a+r $(PRODUCT)-$(VERSION)
	tar chozf $(PRODUCT)-$(VERSION).tar.gz $(PRODUCT)-$(VERSION)
	rm -rf $(PRODUCT)-$(VERSION)

dist-shar: $(DISTFILES) shar
	rm -rf $(PRODUCT)-$(VERSION)
	mkdir $(PRODUCT)-$(VERSION)
	chmod 777 $(PRODUCT)-$(VERSION)
	@echo "Copying distribution files"
	@for file in $(DISTFILES); do \
	  ln $(srcdir)/$$file $(PRODUCT)-$(VERSION) \
	    || cp -p $(srcdir)/$$file $(PRODUCT)-$(VERSION); \
	done
	chmod -R a+r $(PRODUCT)-$(VERSION)
	./shar -cF -L40 -o$(PRODUCT)-$(VERSION).shar $(PRODUCT)-$(VERSION)
	rm -rf $(PRODUCT)-$(VERSION)

# For an explanation of the following Makefile rules, see node
# `Automatic Remaking' in GNU Autoconf documentation.
Makefile: Makefile.in config.status
	CONFIG_FILES=$@ CONFIG_HEADERS= ./config.status
config.status: configure
	./config.status --recheck
configure: configure.in aclocal.m4
	cd $(srcdir) && autoconf

config.h: stamp-h
stamp-h: config.h.in config.status
	CONFIG_FILES= CONFIG_HEADERS=config.h ./config.status
	date > stamp-h
config.h.in: stamp-h.in
stamp-h.in: configure.in aclocal.m4 acconfig.h
	cd $(srcdir) && autoheader
	date > $(srcdir)/stamp-h.in

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
