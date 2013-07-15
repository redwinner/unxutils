# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.1l from Makefile.am

# Copyright (C) 1994, 1995, 1996 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.


SHELL = /bin/sh

srcdir = .
top_srcdir = .
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

pkgdatadir = $(datadir)/sh-utils
pkglibdir = $(libdir)/sh-utils
pkgincludedir = $(includedir)/sh-utils

top_builddir = .

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,
YACC = bison -y
RANLIB = ranlib
GENCAT = 
GMSGFMT = msgfmt
CC = gcc

AUTOMAKE_OPTIONS = gnits 1.1k

SUBDIRS = lib intl src doc man po tests
EXTRA_DIST = ChangeLog.0
ACLOCAL = $(top_srcdir)/aclocal.m4
CONFIG_HEADER_IN = config.h.in
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = config.h
CONFIG_CLEAN_FILES = 
DIST_COMMON =  README ABOUT-NLS AUTHORS COPYING ChangeLog INSTALL \
Makefile.am Makefile.in NEWS README THANKS TODO acconfig.h acinclude.m4 \
aclocal.m4 config.h.in configure configure.in install-sh mkinstalldirs \
stamp-h.in


PACKAGE = sh-utils
VERSION = 1.13

DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) \
	$(TEXINFOS) $(MANS) $(EXTRA_DIST)

TAR = tar
default: all

.SUFFIXES:
$(srcdir)/Makefile.in: #M# Makefile.am $(top_srcdir)/configure.in $(ACLOCAL) 
	cd $(top_srcdir) && automake --gnits Makefile

Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(srcdir)/aclocal.m4: #M#configure.in acinclude.m4
	cd $(srcdir) && aclocal

config.status: configure
	$(SHELL) ./config.status --recheck
$(srcdir)/configure: #M#configure.in $(ACLOCAL) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && autoconf

$(CONFIG_HEADER): stamp-h
stamp-h: $(CONFIG_HEADER_IN) $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=$(CONFIG_HEADER) \
	     $(SHELL) ./config.status
	@echo timestamp > stamp-h
$(srcdir)/$(CONFIG_HEADER_IN): #M#stamp-h.in
$(srcdir)/stamp-h.in: $(top_srcdir)/configure.in $(ACLOCAL) acconfig.h
	cd $(top_srcdir) && autoheader
	echo timestamp > $(srcdir)/stamp-h.in

mostlyclean-hdr:

clean-hdr:

distclean-hdr:
	rm -f $(CONFIG_HEADER)

maintainer-clean-hdr:

# This directory's subdirectories are mostly independent; you can cd
# into them and run `make' without going through this Makefile.
# To change the values of `make' variables: instead of editing Makefiles,
# (1) if the variable is set in `config.status', edit `config.status'
#     (which will cause the Makefiles to be regenerated when you run `make');
# (2) otherwise, pass the desired values on the `make' command line.



all-recursive install-data-recursive install-exec-recursive \
installdirs-recursive install-recursive uninstall-recursive  \
check-recursive installcheck-recursive info-recursive dvi-recursive \
mostlyclean-recursive clean-recursive distclean-recursive \
maintainer-clean-recursive:
	@for subdir in $(SUBDIRS); do \
	  target=`echo $@ | sed s/-recursive//`; \
	  echo "Making $$target in $$subdir"; \
	  (cd $$subdir && $(MAKE) $$target) \
	   || case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
tags-recursive:
	list="$(SUBDIRS)"; for subdir in $$list; do \
	  (cd $$subdir && $(MAKE) tags); \
	done
tags: TAGS
TAGS:


distdir = $(PACKAGE)-$(VERSION)
# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	rm -rf $(distdir)
	$(TAR) zxf $(distdir).tar.gz
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	dc_install_base=`cd $(distdir)/=inst && pwd`; \
	cd $(distdir)/=build \
	  && ../configure --with-included-gettext --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) \
	  && $(MAKE) dvi \
	  && $(MAKE) check \
	  && $(MAKE) install \
	  && $(MAKE) installcheck \
	  && $(MAKE) dist \
	  && $(MAKE) distclean
	rm -rf $(distdir)/=build $(distdir)/=inst
	rm -rf ./=dist
	mkdir ./=dist && cd ./=dist && $(TAR) zxf ../$(distdir).tar.gz
	diff -ur ./=dist/$(distdir) $(distdir)
	rm -rf $(distdir) ./=dist
	@echo "========================"; \
	echo "$(distdir).tar.gz is ready for distribution"; \
	echo "========================"
dist: distdir
	-chmod -R a+r $(distdir)
	$(TAR) chozf $(distdir).tar.gz $(distdir)
	rm -rf $(distdir)
dist-all: distdir
	-chmod -R a+r $(distdir)
	$(TAR) chozf $(distdir).tar.gz $(distdir)
	rm -rf $(distdir)
distdir: $(DISTFILES)
	@if sed 15q $(srcdir)/NEWS | fgrep -e "$(VERSION)" > /dev/null; then :; else \
	  echo "NEWS not updated; not releasing" 1>&2; \
	  exit 1; \
	fi
	rm -rf $(distdir)
	mkdir $(distdir)
	-chmod 755 $(distdir)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  test -f $(distdir)/$$file \
	  || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	  || cp -p $$d/$$file $(distdir)/$$file; \
	done
	for subdir in $(SUBDIRS); do		\
	  test -d $(distdir)/$$subdir		\
	  || mkdir $(distdir)/$$subdir		\
	  || exit 1;				\
	  chmod 755 $(distdir)/$$subdir;	\
	  (cd $$subdir && $(MAKE) distdir=../$(distdir)/$$subdir distdir) \
	    || exit 1; \
	done
info: info-recursive
dvi: dvi-recursive
check: all-am
	$(MAKE) check-recursive
installcheck: installcheck-recursive
all-recursive-am: $(CONFIG_HEADER)
	$(MAKE) all-recursive

all-am: Makefile config.h

install-exec: install-exec-recursive
	$(NORMAL_INSTALL)

install-data: install-data-recursive
	$(NORMAL_INSTALL)

install: install-recursive
	@:

uninstall: uninstall-recursive

all: all-recursive-am all-am

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install
installdirs: installdirs-recursive


mostlyclean-generic:
	test -z "$(MOSTLYCLEANFILES)" || rm -f $(MOSTLYCLEANFILES)

clean-generic:
	test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	rm -f Makefile $(DISTCLEANFILES)
	rm -f config.cache config.log stamp-h
	test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)

maintainer-clean-generic:
	test -z "$(MAINTAINERCLEANFILES)" || rm -f $(MAINTAINERCLEANFILES)
	test -z "$(BUILT_SOURCES)" || rm -f $(BUILT_SOURCES)
mostlyclean-am:  mostlyclean-hdr mostlyclean-generic

clean-am:  clean-hdr clean-generic mostlyclean-am

distclean-am:  distclean-hdr distclean-generic clean-am

maintainer-clean-am:  maintainer-clean-hdr maintainer-clean-generic \
		distclean-am

mostlyclean:  mostlyclean-am mostlyclean-recursive

clean:  clean-am clean-recursive

distclean:  distclean-am distclean-recursive
	rm -f config.status

maintainer-clean:  maintainer-clean-am maintainer-clean-recursive
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f config.status

.PHONY: default mostlyclean-hdr distclean-hdr clean-hdr \
maintainer-clean-hdr install-data-recursive uninstall-data-recursive \
install-exec-recursive uninstall-exec-recursive installdirs-recursive \
uninstalldirs-recursive all-recursive check-recursive \
installcheck-recursive info-recursive dvi-recursive \
mostlyclean-recursive distclean-recursive clean-recursive \
maintainer-clean-recursive tags tags-recursive distdir info dvi \
installcheck all-recursive-am all-am install-exec install-data install \
uninstall all installdirs mostlyclean-generic distclean-generic \
clean-generic maintainer-clean-generic clean mostlyclean distclean \
maintainer-clean


install-root:
	cd src && $(MAKE) $@

maintainer-check:

# Tag before making distribution.  Also, don't make a distribution if
# checks fail.  Also, make sure the NEWS file is up-to-date.
cvs-dist: maintainer-check
	pkg=`echo "sh-utils" | tr a-z A-Z`; \
	ver=`echo "1.13" | sed 's/\./_/g'`; \
	tag="$$pkg-$$ver"; \
	echo tag=$$tag; \
	if cvs -n log -h README| grep -e $$tag > /dev/null; then \
	  echo "VERSION not new; not releasing" 1>&2; \
	  exit 1; \
	else :; \
	fi; \
	cvs tag -c $$tag
	$(MAKE) dist

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
