## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = ansi2knr

EXTRAdir = $(bindir)
EXTRA_PROGRAMS = df mvdir

bin_PROGRAMS = chgrp chown chmod cp dd dircolors du \
ginstall ln dir vdir ls mkdir \
mkfifo mknod mv rm rmdir sync touch @DF_PROG@

libexec_PROGRAMS = @MVDIR_PROG@
datadir = $(prefix)/@DATADIRNAME@
localedir = $(datadir)/locale

noinst_HEADERS = system.h cp.h ls.h dircolors.h
EXTRA_DIST = cp-aux.c cp-hash.c dcgen.pl dircolors.hin

INCLUDES = -I.. -I$(srcdir) -I$(top_srcdir)/lib -I../intl
DEFS = -DLOCALEDIR=\"$(localedir)\" -DSHAREDIR=\"$(datadir)\" @DEFS@
LDADD = @INTLLIBS@ ../lib/libfu.a

$(bin_PROGRAMS) $(libexec_PROGRAMS): ../lib/libfu.a

# Use `ginstall' in the definition of PROGRAMS and in dependencies to avoid
# confusion with the `install' target.  The install rule transforms `ginstall'
# to install before applying any user-specified name transformations.

transform = s/ginstall/install/; @program_transform_name@
ginstall_SOURCES = install.c

cp_SOURCES = cp.c cp-aux.c cp-hash.c
dir_SOURCES = ls.c ls-dir.c
vdir_SOURCES = ls.c ls-vdir.c
ls_SOURCES = ls.c ls-ls.c

BUILT_SOURCES = dircolors.h
PERL = @PERL@
editpl = sed -e 's,@''PERL''@,$(PERL),g'

CLEANFILES = dcgen
MAINTAINERCLEANFILES = dircolors.h

dircolors.h: @MAINT@dcgen dircolors.hin
	./dcgen $(srcdir)/dircolors.hin > $@-t
	mv $@-t $@

SUFFIXES = .pl

.pl:
	rm -f $@ $@.tmp
	$(editpl) $< > $@.tmp && chmod +x $@.tmp && mv $@.tmp $@

