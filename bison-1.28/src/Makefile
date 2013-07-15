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

pkgdatadir = $(datadir)/bison
pkglibdir = $(libdir)/bison
pkgincludedir = $(includedir)/bison

top_builddir = ..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
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
CATALOGS =  de.gmo es.gmo fr.gmo nl.gmo
CATOBJEXT = .gmo
CC = gcc
CPP = gcc -E
DATADIRNAME = share
GENCAT = 
GMOFILES =  de.gmo es.gmo fr.gmo nl.gmo
GMSGFMT = msgfmt
GT_NO = 
GT_YES = #YES#
INCLUDE_LOCALE_H = #include <locale.h>
INSTOBJEXT = .mo
INTLDEPS = $(top_builddir)/intl/libintl.a
INTLLIBS = $(top_builddir)/intl/libintl.a
INTLOBJS = $(GETTOBJS)
MAKEINFO = makeinfo
MKINSTALLDIRS = ./mkinstalldirs
MSGFMT = msgfmt
PACKAGE = bison
POFILES =  de.po es.po fr.po nl.po
POSUB = po
RANLIB = ranlib
U = 
USE_INCLUDED_LIBINTL = yes
USE_NLS = yes
VERSION = 1.28
l = 

AUTOMAKE_OPTIONS = 1.4 ansi2knr

bin_PROGRAMS = bison

bison_SOURCES = LR0.c allocate.c closure.c conflicts.c derives.c	    files.c getargs.c gram.c lalr.c lex.c main.c nullable.c output.c	    print.c reader.c reduce.c symtab.c warshall.c getopt.c getopt1.c


EXTRA_bison_SOURCES = vmsgetargs.c

bison_LDADD = $(top_builddir)/intl/libintl.a 
INCLUDES = -I../intl -I$(top_srcdir)/intl

noinst_HEADERS = alloc.h files.h gram.h lex.h machine.h state.h	 symtab.h system.h types.h getopt.h


data_DATA = bison.simple bison.hairy

EXTRA_DIST = bison.s1 bison.hairy build.com bison.cld vmshlp.mar

DISTCLEANFILES = bison.simple
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
PROGRAMS =  $(bin_PROGRAMS)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I..
CPPFLAGS = 
LDFLAGS = 
LIBS = 
ANSI2KNR = 
bison_OBJECTS =  LR0$U.o allocate$U.o closure$U.o conflicts$U.o \
derives$U.o files$U.o getargs$U.o gram$U.o lalr$U.o lex$U.o main$U.o \
nullable$U.o output$U.o print$U.o reader$U.o reduce$U.o symtab$U.o \
warshall$U.o getopt$U.o getopt1$U.o
bison_DEPENDENCIES =  
bison_LDFLAGS = 
CFLAGS = -g -O2
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
DATA =  $(data_DATA)

HEADERS =  $(noinst_HEADERS)

DIST_COMMON =  ChangeLog Makefile.am Makefile.in alloca.c ansi2knr.1 \
ansi2knr.c


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
SOURCES = $(bison_SOURCES) $(EXTRA_bison_SOURCES)
OBJECTS = $(bison_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu --include-deps src/Makefile

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
	    echo "  $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
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

bison: $(bison_OBJECTS) $(bison_DEPENDENCIES)
	@rm -f bison
	$(LINK) $(bison_LDFLAGS) $(bison_OBJECTS) $(bison_LDADD) $(LIBS)
LR0_.c: LR0.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/LR0.c; then echo $(srcdir)/LR0.c; else echo LR0.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > LR0_.c
alloca_.c: alloca.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/alloca.c; then echo $(srcdir)/alloca.c; else echo alloca.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > alloca_.c
allocate_.c: allocate.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/allocate.c; then echo $(srcdir)/allocate.c; else echo allocate.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > allocate_.c
closure_.c: closure.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/closure.c; then echo $(srcdir)/closure.c; else echo closure.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > closure_.c
conflicts_.c: conflicts.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/conflicts.c; then echo $(srcdir)/conflicts.c; else echo conflicts.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > conflicts_.c
derives_.c: derives.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/derives.c; then echo $(srcdir)/derives.c; else echo derives.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > derives_.c
files_.c: files.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/files.c; then echo $(srcdir)/files.c; else echo files.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > files_.c
getargs_.c: getargs.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/getargs.c; then echo $(srcdir)/getargs.c; else echo getargs.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > getargs_.c
getopt_.c: getopt.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/getopt.c; then echo $(srcdir)/getopt.c; else echo getopt.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > getopt_.c
getopt1_.c: getopt1.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/getopt1.c; then echo $(srcdir)/getopt1.c; else echo getopt1.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > getopt1_.c
gram_.c: gram.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/gram.c; then echo $(srcdir)/gram.c; else echo gram.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > gram_.c
lalr_.c: lalr.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/lalr.c; then echo $(srcdir)/lalr.c; else echo lalr.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > lalr_.c
lex_.c: lex.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/lex.c; then echo $(srcdir)/lex.c; else echo lex.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > lex_.c
main_.c: main.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/main.c; then echo $(srcdir)/main.c; else echo main.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > main_.c
nullable_.c: nullable.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/nullable.c; then echo $(srcdir)/nullable.c; else echo nullable.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > nullable_.c
output_.c: output.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/output.c; then echo $(srcdir)/output.c; else echo output.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > output_.c
print_.c: print.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/print.c; then echo $(srcdir)/print.c; else echo print.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > print_.c
reader_.c: reader.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/reader.c; then echo $(srcdir)/reader.c; else echo reader.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > reader_.c
reduce_.c: reduce.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/reduce.c; then echo $(srcdir)/reduce.c; else echo reduce.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > reduce_.c
symtab_.c: symtab.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/symtab.c; then echo $(srcdir)/symtab.c; else echo symtab.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > symtab_.c
vmsgetargs_.c: vmsgetargs.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/vmsgetargs.c; then echo $(srcdir)/vmsgetargs.c; else echo vmsgetargs.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > vmsgetargs_.c
warshall_.c: warshall.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/warshall.c; then echo $(srcdir)/warshall.c; else echo warshall.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > warshall_.c
LR0_.o alloca_.o allocate_.o closure_.o conflicts_.o derives_.o \
files_.o getargs_.o getopt_.o getopt1_.o gram_.o lalr_.o lex_.o main_.o \
nullable_.o output_.o print_.o reader_.o reduce_.o symtab_.o \
vmsgetargs_.o warshall_.o : $(ANSI2KNR)

install-dataDATA: $(data_DATA)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(datadir)
	@list='$(data_DATA)'; for p in $$list; do \
	  if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(datadir)/$$p"; \
	    $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(datadir)/$$p; \
	  else if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(datadir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(datadir)/$$p; \
	  fi; fi; \
	done

uninstall-dataDATA:
	@$(NORMAL_UNINSTALL)
	list='$(data_DATA)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(datadir)/$$p; \
	done

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
	    cp -pr $$/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
LR0.o: LR0.c system.h ../config.h getopt.h machine.h alloc.h gram.h \
	state.h
allocate.o: allocate.c system.h ../config.h getopt.h
closure.o: closure.c system.h ../config.h getopt.h machine.h alloc.h \
	gram.h
conflicts.o: conflicts.c system.h ../config.h getopt.h machine.h alloc.h \
	files.h gram.h state.h
derives.o: derives.c system.h ../config.h getopt.h alloc.h types.h \
	gram.h
files.o: files.c system.h ../config.h getopt.h files.h alloc.h gram.h
getargs.o: getargs.c getopt.h system.h ../config.h files.h
getopt.o: getopt.c ../config.h
getopt1.o: getopt1.c ../config.h getopt.h
gram.o: gram.c
lalr.o: lalr.c system.h ../config.h getopt.h machine.h types.h state.h \
	alloc.h gram.h
lex.o: lex.c system.h ../config.h getopt.h files.h symtab.h lex.h \
	alloc.h
main.o: main.c system.h ../config.h getopt.h machine.h
nullable.o: nullable.c system.h ../config.h getopt.h types.h gram.h \
	alloc.h
output.o: output.c system.h ../config.h getopt.h machine.h alloc.h \
	files.h gram.h state.h
print.o: print.c system.h ../config.h getopt.h machine.h alloc.h files.h \
	gram.h state.h
reader.o: reader.c system.h ../config.h getopt.h files.h alloc.h \
	symtab.h lex.h gram.h machine.h
reduce.o: reduce.c system.h ../config.h getopt.h files.h gram.h \
	machine.h alloc.h
symtab.o: symtab.c system.h ../config.h getopt.h alloc.h symtab.h gram.h
warshall.o: warshall.c system.h ../config.h getopt.h machine.h

info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am: install-binPROGRAMS
install-exec: install-exec-am

install-data-am: install-dataDATA
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-binPROGRAMS uninstall-dataDATA
uninstall: uninstall-am
all-am: Makefile $(ANSI2KNR) $(PROGRAMS) $(DATA) $(HEADERS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(bindir) $(DESTDIR)$(datadir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*
	-test -z "$(DISTCLEANFILES)" || rm -f $(DISTCLEANFILES)

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
clean-kr maintainer-clean-kr uninstall-dataDATA install-dataDATA tags \
mostlyclean-tags distclean-tags clean-tags maintainer-clean-tags \
distdir info-am info dvi-am dvi check check-am installcheck-am \
installcheck install-exec-am install-exec install-data-am install-data \
install-am install uninstall-am uninstall all-redirect all-am all \
installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


bison.simple: bison.s1 Makefile
	-rm -f $@
	sed -e "/^#line/ s|bison|$(datadir)/bison|" -e "s/@bison_version@/$(VERSION)/" < $(srcdir)/bison.s1 > $@-tmp
	mv $@-tmp $@

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
