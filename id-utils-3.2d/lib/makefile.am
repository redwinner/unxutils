## Process this file with automake to create Makefile.in

AUTOMAKE_OPTIONS = ../src/ansi2knr

noinst_LIBRARIES = libsys.a

EXTRA_DIST = alloca.c basename.c dirname.c error.c fnmatch.c memcpy.c \
	memset.c obstack.c regex.c strcasecmp.c strcspn.c strdup.c \
	strndup.c strpbrk.c strspn.c strstr.c strtok.c

libsys_a_SOURCES = xgetcwd.c xmalloc.c getopt.c getopt1.c

noinst_HEADERS = ansidecl.h error.h fnmatch.h getopt.h obstack.h \
	pathmax.h regex.h xalloca.h xdirent.h xfnmatch.h \
	xmalloc.h xobstack.h xstddef.h xstdlib.h xstring.h xsysstat.h \
	xunistd.h

INCLUDES = -I. -I$(srcdir) \
	   -I../intl -I$(top_srcdir)/intl \
	   -I.. -I$(top_srcdir)
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
libsys_a_LIBADD = @LIBOBJS@ @ALLOCA@
