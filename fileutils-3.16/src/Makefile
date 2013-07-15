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
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/fileutils
pkglibdir = $(libdir)/fileutils
pkgincludedir = $(includedir)/fileutils

top_builddir = ..

ACLOCAL = @ACLOCAL@
AUTOCONF = @AUTOCONF@
AUTOMAKE = @AUTOMAKE@
AUTOHEADER = @AUTOHEADER@

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
CATALOGS =  de.gmo es.gmo fr.gmo nl.gmo pl.gmo pt.gmo sl.gmo sv.gmo
CATOBJEXT = .gmo
CC = gcc
CPP = gcc -E
DATADIRNAME = share
DF_PROG = 
GENCAT = 
GMOFILES =  de.gmo es.gmo fr.gmo nl.gmo pl.gmo pt.gmo sl.gmo sv.gmo
GMSGFMT = msgfmt
GT_NO = 
GT_YES = #YES#
INSTOBJEXT = .mo
INTLDEPS = ../intl/libintl.a
INTLLIBS = ../intl/libintl.a
INTLOBJS = $(GETTOBJS)
LIBOBJS =  regex.o fileblocks.o basename.o euidaccess.o group-member.o rpmatch.o stpcpy.o strndup.o mktime.o strftime.o getline.o error.o obstack.o fnmatch.o
LN = /usr/bin/ln
MAINT = #M#
MKINSTALLDIRS = ./mkinstalldirs
MSGFMT = msgfmt
MV = /usr/bin/mv
MVDIR_PROG = 
PACKAGE = fileutils
POFILES =  de.po es.po fr.po nl.po pl.po pt.po sl.po sv.po
POSUB = po
RANLIB = ranlib
RM = /usr/bin/rm
U = 
USE_INCLUDED_LIBINTL = yes
USE_NLS = yes
VERSION = 3.16
YACC = bison -y

AUTOMAKE_OPTIONS = ansi2knr

EXTRAdir = $(bindir)
EXTRA_PROGRAMS = df mvdir

bin_PROGRAMS = chgrp chown chmod cp dd dircolors du ginstall ln dir vdir ls mkdir mkfifo mknod mv rm rmdir sync touch 


libexec_PROGRAMS = 
datadir = $(prefix)/share
localedir = $(datadir)/locale

noinst_HEADERS = system.h cp.h ls.h dircolors.h
EXTRA_DIST = cp-aux.c cp-hash.c dcgen.pl dircolors.hin

INCLUDES = -I.. -I$(srcdir) -I$(top_srcdir)/lib -I../intl
DEFS = -DLOCALEDIR=\"$(localedir)\" -DSHAREDIR=\"$(datadir)\" -DHAVE_CONFIG_H
LDADD = ../intl/libintl.a ../lib/libfu.a

# Use `ginstall' in the definition of PROGRAMS and in dependencies to avoid
# confusion with the `install' target.  The install rule transforms `ginstall'
# to install before applying any user-specified name transformations.

transform = s/ginstall/install/; s,x,x,
ginstall_SOURCES = install.c

cp_SOURCES = cp.c cp-aux.c cp-hash.c
dir_SOURCES = ls.c ls-dir.c
vdir_SOURCES = ls.c ls-vdir.c
ls_SOURCES = ls.c ls-ls.c

BUILT_SOURCES = dircolors.h
PERL = /usr/local/bin/perl
editpl = sed -e 's,@''PERL''@,$(PERL),g'

CLEANFILES = dcgen
MAINTAINERCLEANFILES = dircolors.h

SUFFIXES = .pl
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
PROGRAMS =  $(bin_PROGRAMS) $(libexec_PROGRAMS)

CPPFLAGS = 
LDFLAGS = 
LIBS = 
ANSI2KNR = 
df_SOURCES = df.c
df_OBJECTS =  df$U.o
df_LDADD = $(LDADD)
df_DEPENDENCIES =  ../lib/libfu.a
df_LDFLAGS = 
mvdir_SOURCES = mvdir.c
mvdir_OBJECTS =  mvdir$U.o
mvdir_LDADD = $(LDADD)
mvdir_DEPENDENCIES =  ../lib/libfu.a
mvdir_LDFLAGS = 
chgrp_SOURCES = chgrp.c
chgrp_OBJECTS =  chgrp$U.o
chgrp_LDADD = $(LDADD)
chgrp_DEPENDENCIES =  ../lib/libfu.a
chgrp_LDFLAGS = 
chown_SOURCES = chown.c
chown_OBJECTS =  chown$U.o
chown_LDADD = $(LDADD)
chown_DEPENDENCIES =  ../lib/libfu.a
chown_LDFLAGS = 
chmod_SOURCES = chmod.c
chmod_OBJECTS =  chmod$U.o
chmod_LDADD = $(LDADD)
chmod_DEPENDENCIES =  ../lib/libfu.a
chmod_LDFLAGS = 
cp_OBJECTS =  cp$U.o cp-aux$U.o cp-hash$U.o
cp_LDADD = $(LDADD)
cp_DEPENDENCIES =  ../lib/libfu.a
cp_LDFLAGS = 
dd_SOURCES = dd.c
dd_OBJECTS =  dd$U.o
dd_LDADD = $(LDADD)
dd_DEPENDENCIES =  ../lib/libfu.a
dd_LDFLAGS = 
dircolors_SOURCES = dircolors.c
dircolors_OBJECTS =  dircolors$U.o
dircolors_LDADD = $(LDADD)
dircolors_DEPENDENCIES =  ../lib/libfu.a
dircolors_LDFLAGS = 
du_SOURCES = du.c
du_OBJECTS =  du$U.o
du_LDADD = $(LDADD)
du_DEPENDENCIES =  ../lib/libfu.a
du_LDFLAGS = 
ginstall_OBJECTS =  install$U.o
ginstall_LDADD = $(LDADD)
ginstall_DEPENDENCIES =  ../lib/libfu.a
ginstall_LDFLAGS = 
ln_SOURCES = ln.c
ln_OBJECTS =  ln$U.o
ln_LDADD = $(LDADD)
ln_DEPENDENCIES =  ../lib/libfu.a
ln_LDFLAGS = 
dir_OBJECTS =  ls$U.o ls-dir$U.o
dir_LDADD = $(LDADD)
dir_DEPENDENCIES =  ../lib/libfu.a
dir_LDFLAGS = 
vdir_OBJECTS =  ls$U.o ls-vdir$U.o
vdir_LDADD = $(LDADD)
vdir_DEPENDENCIES =  ../lib/libfu.a
vdir_LDFLAGS = 
ls_OBJECTS =  ls$U.o ls-ls$U.o
ls_LDADD = $(LDADD)
ls_DEPENDENCIES =  ../lib/libfu.a
ls_LDFLAGS = 
mkdir_SOURCES = mkdir.c
mkdir_OBJECTS =  mkdir$U.o
mkdir_LDADD = $(LDADD)
mkdir_DEPENDENCIES =  ../lib/libfu.a
mkdir_LDFLAGS = 
mkfifo_SOURCES = mkfifo.c
mkfifo_OBJECTS =  mkfifo$U.o
mkfifo_LDADD = $(LDADD)
mkfifo_DEPENDENCIES =  ../lib/libfu.a
mkfifo_LDFLAGS = 
mknod_SOURCES = mknod.c
mknod_OBJECTS =  mknod$U.o
mknod_LDADD = $(LDADD)
mknod_DEPENDENCIES =  ../lib/libfu.a
mknod_LDFLAGS = 
mv_SOURCES = mv.c
mv_OBJECTS =  mv$U.o
mv_LDADD = $(LDADD)
mv_DEPENDENCIES =  ../lib/libfu.a
mv_LDFLAGS = 
rm_SOURCES = rm.c
rm_OBJECTS =  rm$U.o
rm_LDADD = $(LDADD)
rm_DEPENDENCIES =  ../lib/libfu.a
rm_LDFLAGS = 
rmdir_SOURCES = rmdir.c
rmdir_OBJECTS =  rmdir$U.o
rmdir_LDADD = $(LDADD)
rmdir_DEPENDENCIES =  ../lib/libfu.a
rmdir_LDFLAGS = 
sync_SOURCES = sync.c
sync_OBJECTS =  sync$U.o
sync_LDADD = $(LDADD)
sync_DEPENDENCIES =  ../lib/libfu.a
sync_LDFLAGS = 
touch_SOURCES = touch.c
touch_OBJECTS =  touch$U.o
touch_LDADD = $(LDADD)
touch_DEPENDENCIES =  ../lib/libfu.a
touch_LDFLAGS = 
CFLAGS = -g -O2
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
HEADERS =  $(noinst_HEADERS)

DIST_COMMON =  INSTALL Makefile.am Makefile.in ansi2knr.1 ansi2knr.c


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
DEP_FILES =  .deps/chgrp.P .deps/chmod.P .deps/chown.P .deps/cp-aux.P \
.deps/cp-hash.P .deps/cp.P .deps/dd.P .deps/df.P .deps/dircolors.P \
.deps/du.P .deps/install.P .deps/ln.P .deps/ls-dir.P .deps/ls-ls.P \
.deps/ls-vdir.P .deps/ls.P .deps/mkdir.P .deps/mkfifo.P .deps/mknod.P \
.deps/mv.P .deps/mvdir.P .deps/rm.P .deps/rmdir.P .deps/sync.P \
.deps/touch.P
SOURCES = df.c mvdir.c chgrp.c chown.c chmod.c $(cp_SOURCES) dd.c dircolors.c du.c $(ginstall_SOURCES) ln.c $(dir_SOURCES) $(vdir_SOURCES) $(ls_SOURCES) mkdir.c mkfifo.c mknod.c mv.c rm.c rmdir.c sync.c touch.c
OBJECTS = df$U.o mvdir$U.o chgrp$U.o chown$U.o chmod$U.o $(cp_OBJECTS) dd$U.o dircolors$U.o du$U.o $(ginstall_OBJECTS) ln$U.o $(dir_OBJECTS) $(vdir_OBJECTS) $(ls_OBJECTS) mkdir$U.o mkfifo$U.o mknod$U.o mv$U.o rm$U.o rmdir$U.o sync$U.o touch$U.o

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .pl .s
$(srcdir)/Makefile.in: @MAINTAINER_MODE_TRUE@ Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu src/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
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

mostlyclean-libexecPROGRAMS:

clean-libexecPROGRAMS:
	-test -z "$(libexec_PROGRAMS)" || rm -f $(libexec_PROGRAMS)

distclean-libexecPROGRAMS:

maintainer-clean-libexecPROGRAMS:

install-libexecPROGRAMS: $(libexec_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libexecdir)
	@list='$(libexec_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_PROGRAM) $$p $(DESTDIR)$(libexecdir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $$p $(DESTDIR)$(libexecdir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-libexecPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(libexec_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(libexecdir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	done

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

df: $(df_OBJECTS) $(df_DEPENDENCIES)
	@rm -f df
	$(LINK) $(df_LDFLAGS) $(df_OBJECTS) $(df_LDADD) $(LIBS)

mvdir: $(mvdir_OBJECTS) $(mvdir_DEPENDENCIES)
	@rm -f mvdir
	$(LINK) $(mvdir_LDFLAGS) $(mvdir_OBJECTS) $(mvdir_LDADD) $(LIBS)

chgrp: $(chgrp_OBJECTS) $(chgrp_DEPENDENCIES)
	@rm -f chgrp
	$(LINK) $(chgrp_LDFLAGS) $(chgrp_OBJECTS) $(chgrp_LDADD) $(LIBS)

chown: $(chown_OBJECTS) $(chown_DEPENDENCIES)
	@rm -f chown
	$(LINK) $(chown_LDFLAGS) $(chown_OBJECTS) $(chown_LDADD) $(LIBS)

chmod: $(chmod_OBJECTS) $(chmod_DEPENDENCIES)
	@rm -f chmod
	$(LINK) $(chmod_LDFLAGS) $(chmod_OBJECTS) $(chmod_LDADD) $(LIBS)

cp: $(cp_OBJECTS) $(cp_DEPENDENCIES)
	@rm -f cp
	$(LINK) $(cp_LDFLAGS) $(cp_OBJECTS) $(cp_LDADD) $(LIBS)

dd: $(dd_OBJECTS) $(dd_DEPENDENCIES)
	@rm -f dd
	$(LINK) $(dd_LDFLAGS) $(dd_OBJECTS) $(dd_LDADD) $(LIBS)

dircolors: $(dircolors_OBJECTS) $(dircolors_DEPENDENCIES)
	@rm -f dircolors
	$(LINK) $(dircolors_LDFLAGS) $(dircolors_OBJECTS) $(dircolors_LDADD) $(LIBS)

du: $(du_OBJECTS) $(du_DEPENDENCIES)
	@rm -f du
	$(LINK) $(du_LDFLAGS) $(du_OBJECTS) $(du_LDADD) $(LIBS)

ginstall: $(ginstall_OBJECTS) $(ginstall_DEPENDENCIES)
	@rm -f ginstall
	$(LINK) $(ginstall_LDFLAGS) $(ginstall_OBJECTS) $(ginstall_LDADD) $(LIBS)

ln: $(ln_OBJECTS) $(ln_DEPENDENCIES)
	@rm -f ln
	$(LINK) $(ln_LDFLAGS) $(ln_OBJECTS) $(ln_LDADD) $(LIBS)

dir: $(dir_OBJECTS) $(dir_DEPENDENCIES)
	@rm -f dir
	$(LINK) $(dir_LDFLAGS) $(dir_OBJECTS) $(dir_LDADD) $(LIBS)

vdir: $(vdir_OBJECTS) $(vdir_DEPENDENCIES)
	@rm -f vdir
	$(LINK) $(vdir_LDFLAGS) $(vdir_OBJECTS) $(vdir_LDADD) $(LIBS)

ls: $(ls_OBJECTS) $(ls_DEPENDENCIES)
	@rm -f ls
	$(LINK) $(ls_LDFLAGS) $(ls_OBJECTS) $(ls_LDADD) $(LIBS)

mkdir: $(mkdir_OBJECTS) $(mkdir_DEPENDENCIES)
	@rm -f mkdir
	$(LINK) $(mkdir_LDFLAGS) $(mkdir_OBJECTS) $(mkdir_LDADD) $(LIBS)

mkfifo: $(mkfifo_OBJECTS) $(mkfifo_DEPENDENCIES)
	@rm -f mkfifo
	$(LINK) $(mkfifo_LDFLAGS) $(mkfifo_OBJECTS) $(mkfifo_LDADD) $(LIBS)

mknod: $(mknod_OBJECTS) $(mknod_DEPENDENCIES)
	@rm -f mknod
	$(LINK) $(mknod_LDFLAGS) $(mknod_OBJECTS) $(mknod_LDADD) $(LIBS)

mv: $(mv_OBJECTS) $(mv_DEPENDENCIES)
	@rm -f mv
	$(LINK) $(mv_LDFLAGS) $(mv_OBJECTS) $(mv_LDADD) $(LIBS)

rm: $(rm_OBJECTS) $(rm_DEPENDENCIES)
	@rm -f rm
	$(LINK) $(rm_LDFLAGS) $(rm_OBJECTS) $(rm_LDADD) $(LIBS)

rmdir: $(rmdir_OBJECTS) $(rmdir_DEPENDENCIES)
	@rm -f rmdir
	$(LINK) $(rmdir_LDFLAGS) $(rmdir_OBJECTS) $(rmdir_LDADD) $(LIBS)

sync: $(sync_OBJECTS) $(sync_DEPENDENCIES)
	@rm -f sync
	$(LINK) $(sync_LDFLAGS) $(sync_OBJECTS) $(sync_LDADD) $(LIBS)

touch: $(touch_OBJECTS) $(touch_DEPENDENCIES)
	@rm -f touch
	$(LINK) $(touch_LDFLAGS) $(touch_OBJECTS) $(touch_LDADD) $(LIBS)
chgrp_.c: chgrp.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/chgrp.c; then echo $(srcdir)/chgrp.c; else echo chgrp.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > chgrp_.c
chmod_.c: chmod.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/chmod.c; then echo $(srcdir)/chmod.c; else echo chmod.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > chmod_.c
chown_.c: chown.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/chown.c; then echo $(srcdir)/chown.c; else echo chown.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > chown_.c
cp_.c: cp.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/cp.c; then echo $(srcdir)/cp.c; else echo cp.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > cp_.c
cp-aux_.c: cp-aux.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/cp-aux.c; then echo $(srcdir)/cp-aux.c; else echo cp-aux.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > cp-aux_.c
cp-hash_.c: cp-hash.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/cp-hash.c; then echo $(srcdir)/cp-hash.c; else echo cp-hash.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > cp-hash_.c
dd_.c: dd.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/dd.c; then echo $(srcdir)/dd.c; else echo dd.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > dd_.c
df_.c: df.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/df.c; then echo $(srcdir)/df.c; else echo df.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > df_.c
dircolors_.c: dircolors.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/dircolors.c; then echo $(srcdir)/dircolors.c; else echo dircolors.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > dircolors_.c
du_.c: du.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/du.c; then echo $(srcdir)/du.c; else echo du.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > du_.c
install_.c: install.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/install.c; then echo $(srcdir)/install.c; else echo install.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > install_.c
ln_.c: ln.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/ln.c; then echo $(srcdir)/ln.c; else echo ln.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > ln_.c
ls_.c: ls.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/ls.c; then echo $(srcdir)/ls.c; else echo ls.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > ls_.c
ls-dir_.c: ls-dir.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/ls-dir.c; then echo $(srcdir)/ls-dir.c; else echo ls-dir.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > ls-dir_.c
ls-ls_.c: ls-ls.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/ls-ls.c; then echo $(srcdir)/ls-ls.c; else echo ls-ls.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > ls-ls_.c
ls-vdir_.c: ls-vdir.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/ls-vdir.c; then echo $(srcdir)/ls-vdir.c; else echo ls-vdir.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > ls-vdir_.c
mkdir_.c: mkdir.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/mkdir.c; then echo $(srcdir)/mkdir.c; else echo mkdir.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > mkdir_.c
mkfifo_.c: mkfifo.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/mkfifo.c; then echo $(srcdir)/mkfifo.c; else echo mkfifo.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > mkfifo_.c
mknod_.c: mknod.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/mknod.c; then echo $(srcdir)/mknod.c; else echo mknod.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > mknod_.c
mv_.c: mv.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/mv.c; then echo $(srcdir)/mv.c; else echo mv.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > mv_.c
mvdir_.c: mvdir.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/mvdir.c; then echo $(srcdir)/mvdir.c; else echo mvdir.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > mvdir_.c
rm_.c: rm.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/rm.c; then echo $(srcdir)/rm.c; else echo rm.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > rm_.c
rmdir_.c: rmdir.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/rmdir.c; then echo $(srcdir)/rmdir.c; else echo rmdir.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > rmdir_.c
sync_.c: sync.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/sync.c; then echo $(srcdir)/sync.c; else echo sync.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > sync_.c
touch_.c: touch.c $(ANSI2KNR)
	$(CPP) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) `if test -f $(srcdir)/touch.c; then echo $(srcdir)/touch.c; else echo touch.c; fi` | sed 's/^# \([0-9]\)/#line \1/' | $(ANSI2KNR) > touch_.c
chgrp_.o chmod_.o chown_.o cp_.o cp-aux_.o cp-hash_.o dd_.o df_.o \
dircolors_.o du_.o install_.o ln_.o ls_.o ls-dir_.o ls-ls_.o ls-vdir_.o \
mkdir_.o mkfifo_.o mknod_.o mv_.o mvdir_.o rm_.o rmdir_.o sync_.o \
touch_.o : $(ANSI2KNR)

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
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(top_distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu src/Makefile
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

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)

-include $(DEP_FILES)

mostlyclean-depend:

clean-depend:

distclean-depend:
	-rm -rf .deps

maintainer-clean-depend:

%.o: %.c
	@echo '$(COMPILE) -c $<'; \
	$(COMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-cp .deps/$(*F).pp .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm .deps/$(*F).pp

%.lo: %.c
	@echo '$(LTCOMPILE) -c $<'; \
	$(LTCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-sed -e 's/^\([^:]*\)\.o[ 	]*:/\1.lo \1.o :/' \
	  < .deps/$(*F).pp > .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm -f .deps/$(*F).pp
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am: install-binPROGRAMS install-libexecPROGRAMS
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-binPROGRAMS uninstall-libexecPROGRAMS
uninstall: uninstall-am
all-am: Makefile $(ANSI2KNR) $(PROGRAMS) $(HEADERS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(bindir) $(DESTDIR)$(libexecdir)


mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	-test -z "$(BUILT_SOURCES)$(MAINTAINERCLEANFILES)" || rm -f $(BUILT_SOURCES) $(MAINTAINERCLEANFILES)
mostlyclean-am:  mostlyclean-binPROGRAMS mostlyclean-libexecPROGRAMS \
		mostlyclean-compile mostlyclean-krextra mostlyclean-kr \
		mostlyclean-tags mostlyclean-depend mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-binPROGRAMS clean-libexecPROGRAMS clean-compile \
		clean-krextra clean-kr clean-tags clean-depend \
		clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-binPROGRAMS distclean-libexecPROGRAMS \
		distclean-compile distclean-krextra distclean-kr \
		distclean-tags distclean-depend distclean-generic \
		clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-binPROGRAMS \
		maintainer-clean-libexecPROGRAMS \
		maintainer-clean-compile maintainer-clean-krextra \
		maintainer-clean-kr maintainer-clean-tags \
		maintainer-clean-depend maintainer-clean-generic \
		distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-binPROGRAMS distclean-binPROGRAMS clean-binPROGRAMS \
maintainer-clean-binPROGRAMS uninstall-binPROGRAMS install-binPROGRAMS \
mostlyclean-libexecPROGRAMS distclean-libexecPROGRAMS \
clean-libexecPROGRAMS maintainer-clean-libexecPROGRAMS \
uninstall-libexecPROGRAMS install-libexecPROGRAMS mostlyclean-compile \
distclean-compile clean-compile maintainer-clean-compile \
mostlyclean-krextra distclean-krextra clean-krextra \
maintainer-clean-krextra mostlyclean-kr distclean-kr clean-kr \
maintainer-clean-kr tags mostlyclean-tags distclean-tags clean-tags \
maintainer-clean-tags distdir mostlyclean-depend distclean-depend \
clean-depend maintainer-clean-depend info-am info dvi-am dvi check \
check-am installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


$(bin_PROGRAMS) $(libexec_PROGRAMS): ../lib/libfu.a

dircolors.h: #M#dcgen dircolors.hin
	./dcgen $(srcdir)/dircolors.hin > $@-t
	mv $@-t $@

.pl:
	rm -f $@ $@.tmp
	$(editpl) $< > $@.tmp && chmod +x $@.tmp && mv $@.tmp $@

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
