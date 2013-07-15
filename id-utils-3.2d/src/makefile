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


AUTOMAKE_OPTIONS = ansi2knr

bin_PROGRAMS = mkid lid fid fnid xtokid
bin_SCRIPTS = eid aid gid defid

EXTRA_DIST = ansi2knr.1 ansi2knr.c $(bin_SCRIPTS)

localedir = $(datadir)/locale

INCLUDES = -I. -I$(srcdir) 	   -I../libidu -I$(top_srcdir)/libidu 	   -I../lib -I$(top_srcdir)/lib 	   -I../intl -I$(top_srcdir)/intl 	   -I.. -I$(top_srcdir) 

DEFS = -DLOCALEDIR=\"$(localedir)\" -DHAVE_CONFIG_H
LDADD = $(top_builddir)/intl/libintl.a ../libidu/libidu.a ../lib/libsys.a
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
PROGRAMS =  $(bin_PROGRAMS)

CPPFLAGS = 
LIBS = 
ANSI2KNR = 
mkid_SOURCES = mkid.c
mkid_OBJECTS =  mkid$U.o
mkid_LDADD = $(LDADD)
mkid_DEPENDENCIES =  ../libidu/libidu.a ../lib/libsys.a
mkid_LDFLAGS = 
lid_SOURCES = lid.c
lid_OBJECTS =  lid$U.o
lid_LDADD = $(LDADD)
lid_DEPENDENCIES =  ../libidu/libidu.a ../lib/libsys.a
lid_LDFLAGS = 
fid_SOURCES = fid.c
fid_OBJECTS =  fid$U.o
fid_LDADD = $(LDADD)
fid_DEPENDENCIES =  ../libidu/libidu.a ../lib/libsys.a
fid_LDFLAGS = 
fnid_SOURCES = fnid.c
fnid_OBJECTS =  fnid$U.o
fnid_LDADD = $(LDADD)
fnid_DEPENDENCIES =  ../libidu/libidu.a ../lib/libsys.a
fnid_LDFLAGS = 
xtokid_SOURCES = xtokid.c
xtokid_OBJECTS =  xtokid$U.o
xtokid_LDADD = $(LDADD)
xtokid_DEPENDENCIES =  ../libidu/libidu.a ../lib/libsys.a
xtokid_LDFLAGS = 
SCRIPTS =  $(bin_SCRIPTS)

COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  Makefile.am Makefile.in ansi2knr.1 ansi2knr.c


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
SOURCES = mkid.c lid.c fid.c fnid.c xtokid.c
OBJECTS = mkid$U.o lid$U.o fid$U.o fnid$U.o xtokid$U.o

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnits --include-deps src/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-binPROGRAMS:

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

distclean-binPROGRAMS:

maintainer-clean-binPROGRAMS:

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_PROGRAM) $(INSTALL_STRIP_FLAG) $$p $(DESTDIR)$(bindir)/`echo $$p|sed -e 's/$(EXEEXT)$$//' -e '$(transform)' -e 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $(INSTALL_STRIP_FLAG) $$p $(DESTDIR)$(bindir)/`echo $$p|sed -e 's/$(EXEEXT)$$//' -e '$(transform)' -e 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed -e 's/$(EXEEXT)$$//' -e '$(transform)' -e 's/$$/$(EXEEXT)/'`; \
	done

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

mostlyclean-krextra:

clean-krextra:
	-rm -f ansi2knr

distclean-krextra:

maintainer-clean-krextra:
ansi2knr: ansi2knr.o
	$(LINK) ansi2knr.o $(LIBS)
ansi2knr.o: $(CONFIG_HEADER)


mostlyclean-kr:
	-rm -f *_.c

clean-kr:

distclean-kr:

maintainer-clean-kr:

mkid: $(mkid_OBJECTS) $(mkid_DEPENDENCIES)
	@rm -f mkid
	$(LINK) $(mkid_LDFLAGS) $(mkid_OBJECTS) $(mkid_LDADD) $(LIBS)

lid: $(lid_OBJECTS) $(lid_DEPENDENCIES)
	@rm -f lid
	$(LINK) $(lid_LDFLAGS) $(lid_OBJECTS) $(lid_LDADD) $(LIBS)

fid: $(fid_OBJECTS) $(fid_DEPENDENCIES)
	@rm -f fid
	$(LINK) $(fid_LDFLAGS) $(fid_OBJECTS) $(fid_LDADD) $(LIBS)

fnid: $(fnid_OBJECTS) $(fnid_DEPENDENCIES)
	@rm -f fnid
	$(LINK) $(fnid_LDFLAGS) $(fnid_OBJECTS) $(fnid_LDADD) $(LIBS)

xtokid: $(xtokid_OBJECTS) $(xtokid_DEPENDENCIES)
	@rm -f xtokid
	$(LINK) $(xtokid_LDFLAGS) $(xtokid_OBJECTS) $(xtokid_LDADD) $(LIBS)

install-binSCRIPTS: $(bin_SCRIPTS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_SCRIPTS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(INSTALL_SCRIPT) $$p $(DESTDIR)$(bindir)/`echo $$p|sed '$(transform)'`"; \
	    $(INSTALL_SCRIPT) $$p $(DESTDIR)$(bindir)/`echo $$p|sed '$(transform)'`; \
	  else if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_SCRIPT) $(srcdir)/$$p $(DESTDIR)$(bindir)/`echo $$p|sed '$(transform)'`"; \
	    $(INSTALL_SCRIPT) $(srcdir)/$$p $(DESTDIR)$(bindir)/`echo $$p|sed '$(transform)'`; \
	  else :; fi; fi; \
	done

uninstall-binSCRIPTS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_SCRIPTS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed '$(transform)'`; \
	done
fid_.c: fid.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/fid.c; then echo $(srcdir)/fid.c; else echo fid.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > fid_.c
fnid_.c: fnid.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/fnid.c; then echo $(srcdir)/fnid.c; else echo fnid.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > fnid_.c
lid_.c: lid.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/lid.c; then echo $(srcdir)/lid.c; else echo lid.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > lid_.c
mkid_.c: mkid.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/mkid.c; then echo $(srcdir)/mkid.c; else echo mkid.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > mkid_.c
xtokid_.c: xtokid.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/xtokid.c; then echo $(srcdir)/xtokid.c; else echo xtokid.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > xtokid_.c
fid_.o fnid_.o lid_.o mkid_.o xtokid_.o : $(ANSI2KNR)

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

subdir = src

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
install-exec-am: install-binPROGRAMS install-binSCRIPTS
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-binPROGRAMS uninstall-binSCRIPTS
uninstall: uninstall-am
all-am: Makefile $(ANSI2KNR) $(PROGRAMS) $(SCRIPTS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_STRIP_FLAG=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(bindir) $(DESTDIR)$(bindir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-binPROGRAMS mostlyclean-compile \
		mostlyclean-krextra mostlyclean-kr mostlyclean-tags \
		mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-binPROGRAMS clean-compile clean-krextra clean-kr \
		clean-tags clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-binPROGRAMS distclean-compile distclean-krextra \
		distclean-kr distclean-tags distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-binPROGRAMS \
		maintainer-clean-compile maintainer-clean-krextra \
		maintainer-clean-kr maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-binPROGRAMS distclean-binPROGRAMS clean-binPROGRAMS \
maintainer-clean-binPROGRAMS uninstall-binPROGRAMS install-binPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile mostlyclean-krextra distclean-krextra \
clean-krextra maintainer-clean-krextra mostlyclean-kr distclean-kr \
clean-kr maintainer-clean-kr uninstall-binSCRIPTS install-binSCRIPTS \
tags mostlyclean-tags distclean-tags clean-tags maintainer-clean-tags \
distdir info-am info dvi-am dvi check check-am installcheck-am \
installcheck install-exec-am install-exec install-data-am install-data \
install-am install uninstall-am uninstall all-redirect all-am all \
installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


$(PROGRAMS): $(LDADD)

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
