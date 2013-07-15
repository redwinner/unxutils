## Process this file with automake to produce Makefile.in
noinst_LIBRARIES = libsu.a

## FIXME: Remove basename.c when automake is fixed -- probably for 1.1g
EXTRA_DIST = basename.c getloadavg.c getline.c mktime.c strftime.c

INCLUDES = -I.. -I$(srcdir) -I../intl

libsu_a_SOURCES = regex.c getdate.y posixtm.y readtokens.c \
  canon-host.c full-write.c \
  getopt.c getopt1.c getugroups.c long-options.c \
  putenv.c stripslash.c xgetcwd.c xgethostname.c xmalloc.c \
  xstrdup.c xstrtod.c xstrtol.c xstrtoul.c

libsu_a_LIBADD = @LIBOBJS@ @ALLOCA@
libsu_a_DEPENDENCIES = $(libsu_a_LIBADD)

noinst_HEADERS = error.h getline.h getopt.h long-options.h \
  regex.h pathmax.h getdate.c posixtm.c group-member.h readtokens.h \
  xstrtod.h xstrtol.h xstrtoul.h

BUILT_SOURCES = getdate.c posixtm.c
