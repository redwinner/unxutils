## Process this file with automake to create Makefile.in

AUTOMAKE_OPTIONS = ../src/ansi2knr

noinst_LIBRARIES = libidu.a

libidu_a_SOURCES = dynvec.c hash.c idfile.c idread.c idwrite.c fnprint.c \
	scanners.c walker.c

idudir = $(prefix)/share
idu_DATA = id-lang.map
EXTRA_DIST = $(idu_DATA)

noinst_HEADERS = dynvec.h hash.h idfile.h scanners.h tokflags.h xnls.h

INCLUDES = -I. -I$(srcdir) \
	   -I../lib -I$(top_srcdir)/lib \
	   -I../intl -I$(top_srcdir)/intl \
	   -I.. -I$(top_srcdir)
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
