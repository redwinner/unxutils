# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# Makefile for `libiconv' part of `recode'.
# Copyright © 1999, 2000 Free Software Foundation, Inc.
# François Pinard <pinard@iro.umontreal.ca>

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
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.


SHELL = /bin/sh

srcdir = .
top_srcdir = ..
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

DESTDIR =

pkgdatadir = $(datadir)/recode
pkglibdir = $(libdir)/recode
pkgincludedir = $(includedir)/recode

top_builddir = ..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = i686-pc-cygwin
host_triplet = i686-pc-cygwin
ALL_LINGUAS = da de el es fr gl it nl pl pt sl sv
ALL_MOFILES =  da.mo de.mo el.mo es.mo fr.mo gl.mo it.mo nl.mo pl.mo pt.mo sl.mo sv.mo
ALL_POFILES =  da.po de.po el.po es.po fr.po gl.po it.po nl.po pl.po pt.po sl.po sv.po
AS = @AS@
AT_TESTPATH = ../src
CC = gcc
CPP = gcc -E
DLLTOOL = @DLLTOOL@
ECHO_C = 
ECHO_N = -n
GLIBC21 = no
LEX = flex
LEX_OUTPUT_ROOT = lex.yy
LIBOBJS = gettext$U.o
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
LTALLOCA = 
LTLIBOBJS = gettext$U.lo
MAKEINFO = makeinfo
MOFILES =  da.mo de.mo el.mo es.mo fr.mo gl.mo it.mo nl.mo pl.mo pt.mo sl.mo sv.mo
OBJDUMP = @OBJDUMP@
PACKAGE = recode
RANLIB = ranlib
U = 
VERSION = 3.6

AUTOMAKE_OPTIONS = gnits

noinst_HEADERS = iconv.h aliases.h armscii_8.h ascii.h big5.h big5hkscs.h ces_big5.h ces_gbk.h cjk_variants.h cns11643.h cns11643_1.h cns11643_2.h cns11643_3.h cns11643_inv.h converters.h cp1133.h cp1250.h cp1251.h cp1252.h cp1253.h cp1254.h cp1255.h cp1256.h cp1257.h cp1258.h cp850.h cp866.h cp874.h cp932.h cp932ext.h cp936ext.h cp949.h cp950.h cp950ext.h euc_cn.h euc_jp.h euc_kr.h euc_tw.h flags.h gb12345.h gb12345ext.h gb18030.h gb18030ext.h gb18030uni.h gb2312.h gbk.h gbkext1.h gbkext2.h gbkext_inv.h georgian_academy.h georgian_ps.h hkscs.h hp_roman8.h hz.h iso2022_cn.h iso2022_cnext.h iso2022_jp.h iso2022_jp1.h iso2022_jp2.h iso2022_kr.h iso646_cn.h iso646_jp.h iso8859_1.h iso8859_10.h iso8859_13.h iso8859_14.h iso8859_15.h iso8859_16.h iso8859_2.h iso8859_3.h iso8859_4.h iso8859_5.h iso8859_6.h iso8859_7.h iso8859_8.h iso8859_9.h isoir165.h isoir165ext.h java.h jisx0201.h jisx0208.h jisx0212.h johab.h johab_hangul.h koi8_r.h koi8_ru.h koi8_u.h ksc5601.h loops.h loop_unicode.h loop_wchar.h mac_arabic.h mac_centraleurope.h mac_croatian.h mac_cyrillic.h mac_greek.h mac_hebrew.h mac_iceland.h mac_roman.h mac_romania.h mac_thai.h mac_turkish.h mac_ukraine.h mulelao.h nextstep.h sjis.h tcvn.h tis620.h translit.h ucs2.h ucs2be.h ucs2le.h ucs2internal.h ucs2swapped.h ucs4.h ucs4be.h ucs4le.h ucs4internal.h ucs4swapped.h uhc_1.h uhc_2.h utf16.h utf16be.h utf16le.h utf7.h utf8.h viscii.h libcharset.h


EXTRA_DIST = encodings.def encodings_local.def config.charset ref-add.sin ref-del.sin


charset_alias = $(DESTDIR)$(libdir)/charset.alias
charset_tmp = $(DESTDIR)$(libdir)/charset.tmp

SUFFIXES = .sed .sin

CLEANFILES = charset.alias ref-add.sed ref-del.sed
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
CFLAGS = -g -O2
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(LIBTOOL) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
HEADERS =  $(noinst_HEADERS)

DIST_COMMON =  README ChangeLog Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
all: all-redirect
.SUFFIXES:
.SUFFIXES: .sed .sin
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnits --include-deps libiconv/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


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

subdir = libiconv

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$/$$file $(distdir)/$$file; \
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
install-exec-am: install-exec-local
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-local
uninstall: uninstall-am
all-am: Makefile $(HEADERS) all-local
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:


mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-tags clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-tags distclean-generic clean-am
	-rm -f libtool

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-tags maintainer-clean-generic \
		distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: tags mostlyclean-tags distclean-tags clean-tags \
maintainer-clean-tags distdir info-am info dvi-am dvi check check-am \
installcheck-am installcheck install-exec-local install-exec-am \
install-exec install-data-am install-data install-am install \
uninstall-local uninstall-am uninstall all-local all-redirect all-am \
all installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


all-local: charset.alias ref-add.sed ref-del.sed
install-exec-local: all-local
	$(mkinstalldirs) $(DESTDIR)$(libdir)
	if test -f $(charset_alias); then \
	  sed -f ref-add.sed $(charset_alias) > $(charset_tmp) ; \
	  $(INSTALL_DATA) $(charset_tmp) $(charset_alias) ; \
	  rm -f $(charset_tmp) ; \
	else \
	  if test no = no; then \
	    sed -f ref-add.sed charset.alias > $(charset_tmp) ; \
	    $(INSTALL_DATA) $(charset_tmp) $(charset_alias) ; \
	    rm -f $(charset_tmp) ; \
	  fi ; \
	fi

uninstall-local: all-local
	if test -f $(charset_alias); then \
	  sed -f ref-del.sed $(charset_alias) > $(charset_tmp); \
	  if grep '^# Packages using this file: $$' $(charset_tmp) \
	      > /dev/null; then \
	    rm -f $(charset_alias); \
	  else \
	    $(INSTALL_DATA) $(charset_tmp) $(charset_alias); \
	  fi; \
	  rm -f $(charset_tmp); \
	fi

charset.alias: config.charset
	$(SHELL) $(srcdir)/config.charset 'i686-pc-cygwin' > t-$@
	mv t-$@ $@
.sin.sed:
	sed -e '/^#/d' -e 's/@''PACKAGE''@/recode/g' $< > $@-t
	mv $@-t $@

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
