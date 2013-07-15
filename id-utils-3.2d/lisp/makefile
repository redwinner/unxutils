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


lisp_LISP = id-utils.el
EXTRA_DIST = $(lisp_LISP)
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
LISP =  $(lisp_LISP)

ELCFILES =  id-utils.elc
DIST_COMMON =  Makefile.am Makefile.in elisp-comp


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
all: all-redirect
.SUFFIXES:
.SUFFIXES: .el .elc
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnits --include-deps lisp/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


install-lispLISP: $(lisp_LISP) $(ELCFILES)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(lispdir)
	@list='$(lisp_LISP)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(lispdir)/$$p"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(lispdir)/$$p; \
	  if test -f $${p}c; then \
	    echo " $(INSTALL_DATA) $${p}c $(DESTDIR)$(lispdir)/$${p}c"; \
	    $(INSTALL_DATA) $${p}c $(DESTDIR)$(lispdir)/$${p}c; \
	  else : ; fi; \
	done

uninstall-lispLISP:
	@$(NORMAL_UNINSTALL)
	list='$(lisp_LISP)'; for p in $$list; do \
	  rm -f $(lispdir)/$$p $(DESTDIR)$(lispdir)/$${p}c; \
	done
.el.elc:
	@echo 'WARNING: Warnings can be ignored. :-)'
	if test $(EMACS) != no; then \
	  EMACS=$(EMACS) $(SHELL) $(srcdir)/elisp-comp $<; \
	fi

mostlyclean-lisp:

clean-lisp:
	-test -z "$(ELCFILES)" || rm -f $(ELCFILES)

distclean-lisp:

maintainer-clean-lisp:
tags: TAGS
TAGS:


distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = lisp

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

install-data-am: install-lispLISP
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-lispLISP
uninstall: uninstall-am
all-am: Makefile $(LISP) $(ELCFILES)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_STRIP_FLAG=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(lispdir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-lisp mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-lisp clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-lisp distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-lisp maintainer-clean-generic \
		distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: uninstall-lispLISP install-lispLISP mostlyclean-lisp \
distclean-lisp clean-lisp maintainer-clean-lisp tags distdir info-am \
info dvi-am dvi check check-am installcheck-am installcheck \
install-exec-am install-exec install-data-am install-data install-am \
install uninstall-am uninstall all-redirect all-am all installdirs \
mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
