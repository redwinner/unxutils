# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4a from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

SHELL = /bin/sh

srcdir = .
top_srcdir = ..
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = /usr/local/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/id-utils
pkglibdir = $(libdir)/id-utils
pkgincludedir = $(includedir)/id-utils

top_builddir = ..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
INSTALL_STRIP_FLAG =
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
CATALOGS =  de.gmo fr.gmo
CATOBJEXT = .gmo
CC = gcc
CFLAGS = -g -O2
CPP = gcc -E
DATADIRNAME = share
EMACS = no
GENCAT = 
GMOFILES =  de.gmo fr.gmo
GMSGFMT = msgfmt
GT_NO = 
GT_YES = #YES#
INCLUDE_LOCALE_H = #include <locale.h>
INSTOBJEXT = .mo
INTLDEPS = $(top_builddir)/intl/libintl.a
INTLLIBS = $(top_builddir)/intl/libintl.a
INTLOBJS = $(GETTOBJS)
LANGUAGE_MAP_FILE = /usr/local/share/id-lang.map
LDFLAGS = -g
LN_S = ln -s
MAKEINFO = makeinfo
MKINSTALLDIRS = ./mkinstalldirs
MSGFMT = msgfmt
PACKAGE = id-utils
POFILES =  de.po fr.po
POSUB = po
RANLIB = ranlib
README_ALPHA = README-alpha
U = 
USE_INCLUDED_LIBINTL = yes
USE_NLS = yes
VERSION = 3.2d
l = 
lispdir = 


AUTOMAKE_OPTIONS = ../src/ansi2knr

noinst_LIBRARIES = libsys.a

EXTRA_DIST = alloca.c basename.c dirname.c error.c fnmatch.c memcpy.c 	memset.c obstack.c regex.c strcasecmp.c strcspn.c strdup.c 	strndup.c strpbrk.c strspn.c strstr.c strtok.c


libsys_a_SOURCES = xgetcwd.c xmalloc.c getopt.c getopt1.c

noinst_HEADERS = ansidecl.h error.h fnmatch.h getopt.h obstack.h 	pathmax.h regex.h xalloca.h xdirent.h xfnmatch.h 	xmalloc.h xobstack.h xstddef.h xstdlib.h xstring.h xsysstat.h 	xunistd.h


INCLUDES = -I. -I$(srcdir) 	   -I../intl -I$(top_srcdir)/intl 	   -I.. -I$(top_srcdir)

DEFS = -DLOCALEDIR=\"$(localedir)\" -DHAVE_CONFIG_H
libsys_a_LIBADD = obstack$U.o strndup$U.o basename$U.o dirname$U.o fnmatch$U.o error$U.o regex$U.o 
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
LIBRARIES =  $(noinst_LIBRARIES)

CPPFLAGS = 
LIBS = 
ANSI2KNR = ../src/ansi2knr
libsys_a_DEPENDENCIES =  obstack$U.o strndup$U.o basename$U.o dirname$U.o fnmatch$U.o error$U.o regex$U.o 
libsys_a_OBJECTS =  xgetcwd$U.o xmalloc$U.o getopt$U.o getopt1$U.o
AR = ar
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
HEADERS =  $(noinst_HEADERS)

DIST_COMMON =  Makefile.am Makefile.in alloca.c basename.c dirname.c \
error.c fnmatch.c memcpy.c memset.c obstack.c obstack.h regex.c \
strcspn.c strdup.c strndup.c strpbrk.c strspn.c strstr.c strtok.c


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
SOURCES = $(libsys_a_SOURCES)
OBJECTS = $(libsys_a_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnits --include-deps lib/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-noinstLIBRARIES:

clean-noinstLIBRARIES:
	-test -z "$(noinst_LIBRARIES)" || rm -f $(noinst_LIBRARIES)

distclean-noinstLIBRARIES:

maintainer-clean-noinstLIBRARIES:

.c.o:
	$(COMPILE) -c $<

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:
../src/ansi2knr: ../src/ansi2knr.o
	cd ../src && $(MAKE) $(AM_MAKEFLAGS) ansi2knr

../src/ansi2knr.o:
	cd ../src && $(MAKE) $(AM_MAKEFLAGS) ansi2knr.o


mostlyclean-kr:
	-rm -f *_.c

clean-kr:

distclean-kr:

maintainer-clean-kr:

libsys.a: $(libsys_a_OBJECTS) $(libsys_a_DEPENDENCIES)
	-rm -f libsys.a
	$(AR) cru libsys.a $(libsys_a_OBJECTS) $(libsys_a_LIBADD)
	$(RANLIB) libsys.a
alloca_.c: alloca.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/alloca.c; then echo $(srcdir)/alloca.c; else echo alloca.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > alloca_.c
basename_.c: basename.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/basename.c; then echo $(srcdir)/basename.c; else echo basename.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > basename_.c
dirname_.c: dirname.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/dirname.c; then echo $(srcdir)/dirname.c; else echo dirname.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > dirname_.c
error_.c: error.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/error.c; then echo $(srcdir)/error.c; else echo error.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > error_.c
fnmatch_.c: fnmatch.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/fnmatch.c; then echo $(srcdir)/fnmatch.c; else echo fnmatch.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > fnmatch_.c
getopt_.c: getopt.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/getopt.c; then echo $(srcdir)/getopt.c; else echo getopt.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > getopt_.c
getopt1_.c: getopt1.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/getopt1.c; then echo $(srcdir)/getopt1.c; else echo getopt1.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > getopt1_.c
memcpy_.c: memcpy.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/memcpy.c; then echo $(srcdir)/memcpy.c; else echo memcpy.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > memcpy_.c
memset_.c: memset.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/memset.c; then echo $(srcdir)/memset.c; else echo memset.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > memset_.c
obstack_.c: obstack.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/obstack.c; then echo $(srcdir)/obstack.c; else echo obstack.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > obstack_.c
regex_.c: regex.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/regex.c; then echo $(srcdir)/regex.c; else echo regex.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > regex_.c
strcspn_.c: strcspn.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/strcspn.c; then echo $(srcdir)/strcspn.c; else echo strcspn.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > strcspn_.c
strdup_.c: strdup.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/strdup.c; then echo $(srcdir)/strdup.c; else echo strdup.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > strdup_.c
strndup_.c: strndup.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/strndup.c; then echo $(srcdir)/strndup.c; else echo strndup.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > strndup_.c
strpbrk_.c: strpbrk.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/strpbrk.c; then echo $(srcdir)/strpbrk.c; else echo strpbrk.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > strpbrk_.c
strspn_.c: strspn.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/strspn.c; then echo $(srcdir)/strspn.c; else echo strspn.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > strspn_.c
strstr_.c: strstr.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/strstr.c; then echo $(srcdir)/strstr.c; else echo strstr.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > strstr_.c
strtok_.c: strtok.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/strtok.c; then echo $(srcdir)/strtok.c; else echo strtok.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > strtok_.c
xgetcwd_.c: xgetcwd.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/xgetcwd.c; then echo $(srcdir)/xgetcwd.c; else echo xgetcwd.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > xgetcwd_.c
xmalloc_.c: xmalloc.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/xmalloc.c; then echo $(srcdir)/xmalloc.c; else echo xmalloc.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > xmalloc_.c
alloca_.o basename_.o dirname_.o error_.o fnmatch_.o getopt_.o \
getopt1_.o memcpy_.o memset_.o obstack_.o regex_.o strcspn_.o strdup_.o \
strndup_.o strpbrk_.o strspn_.o strstr_.o strtok_.o xgetcwd_.o \
xmalloc_.o : $(ANSI2KNR)

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags  $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = lib

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done

info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am:
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am:
uninstall: uninstall-am
all-am: Makefile $(LIBRARIES) $(HEADERS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_STRIP_FLAG=-s install
installdirs:


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-noinstLIBRARIES mostlyclean-compile \
		mostlyclean-kr mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-noinstLIBRARIES clean-compile clean-kr clean-tags \
		clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-noinstLIBRARIES distclean-compile distclean-kr \
		distclean-tags distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-noinstLIBRARIES \
		maintainer-clean-compile maintainer-clean-kr \
		maintainer-clean-tags maintainer-clean-generic \
		distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-noinstLIBRARIES distclean-noinstLIBRARIES \
clean-noinstLIBRARIES maintainer-clean-noinstLIBRARIES \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile mostlyclean-kr distclean-kr clean-kr \
maintainer-clean-kr tags mostlyclean-tags distclean-tags clean-tags \
maintainer-clean-tags distdir info-am info dvi-am dvi check check-am \
installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
