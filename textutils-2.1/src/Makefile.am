## Process this file with automake to produce Makefile.in -*-Makefile-*-
AUTOMAKE_OPTIONS = ansi2knr

bin_PROGRAMS = cat cksum comm csplit cut expand fmt fold head join md5sum \
nl od paste pr ptx sha1sum sort split sum tac tail tr tsort unexpand uniq wc

noinst_HEADERS = sys2.h system.h checksum.h
EXTRA_DIST = tac-pipe.c

localedir = $(datadir)/locale

INCLUDES = -I.. -I$(srcdir) -I$(top_srcdir)/lib -I../lib
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

# Sometimes, the expansion of @LIBINTL@ includes -lc which may
# include modules defining variables like `optind', so libfetish.a
# must precede @LIBINTL@ in order to ensure we use GNU getopt.
# But libfetish.a must also follow @LIBINTL@, since libintl uses
# replacement functions defined in libfetish.a.
LDADD = ../lib/libfetish.a @LIBINTL@ ../lib/libfetish.a

$(PROGRAMS): ../lib/libfetish.a

md5sum_SOURCES = md5sum.c md5.c
sha1sum_SOURCES = md5sum.c sha1sum.c

## If necessary, add -lm to resolve use of pow in lib/strtod.c.
sort_LDADD = $(LDADD) @POW_LIB@
