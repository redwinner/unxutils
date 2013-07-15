# Generated automatically from Makefile.in by configure.
# Makefile for GNU shell utilities programs.
# Copyright (C) 1991, 1992, 1993, 1994 Free Software Foundation, Inc.

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
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

SHELL = /bin/sh

srcdir = .

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
binprefix = 

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
CC = gcc
DEFS = -DCONFIG_BROKETS -DHAVE_CONFIG_H
LIBS = 
CFLAGS = -g -O
LDFLAGS = 

SOURCES = basename.c date.c dirname.c echo.c env.c expr.c id.c logname.c \
long-options.c pathchk.c printenv.c printf.c sleep.c su.c tee.c test.c tty.c \
who.c whoami.c yes.c nice.c stty.c uname.c version.c

OBJECTS = basename.o date.o dirname.o echo.o env.o expr.o id.o logname.o \
long-options.o pathchk.o printenv.o printf.o sleep.o su.o tee.o test.o tty.o \
who.o whoami.o yes.o nice.o stty.o uname.o version.o

DISTFILES = Makefile.in system.h $(SOURCES) groups.sh nohup.sh \
false.sh true.sh version.h long-options.h

PROGS = basename date dirname echo env expr false groups id logname \
pathchk printenv printf sleep tee test true tty who whoami yes 

all: $(PROGS) su

incl = -I.. -I$(srcdir) -I$(srcdir)/../lib
.c.o:
	$(CC) -c $(CPPFLAGS) $(DEFS) $(incl) $(CFLAGS) $<

install: all
	for f in $(PROGS); do \
	    $(INSTALL_PROGRAM) $$f $(bindir)/$(binprefix)$$f; \
	done
	rm -f $(bindir)/'['; ln $(bindir)/$(binprefix)test $(bindir)/'['
	@if test `./id -u` = 0; then \
	echo you are root, so installing su; \
	$(INSTALL_PROGRAM) su $(bindir)/$(binprefix)su; \
	chown root $(bindir)/$(binprefix)su; \
	chmod 4755 $(bindir)/$(binprefix)su; \
	else echo WARNING: you are not root, so not installing su; fi

uninstall:
	for f in $(PROGS) su; do rm -f $(bindir)/$(binprefix)$$f; done
	rm -f $(bindir)/'['

TAGS: $(SOURCES)
	etags $(SOURCES)

check:

clean:
	rm -f $(PROGS) su *.o core '[' .version

mostlyclean: clean

distclean: clean
	rm -f Makefile

realclean: distclean
	rm -f TAGS

dist: $(DISTFILES)
	ln $(DISTFILES) ../`cat ../.fname`/src

.version: version.c
	sed -e '/version_string/!d' \
	    -e 's/[^"]*"\([^"]*\)".*/\1/' \
	    -e q $(srcdir)/version.c \
	  > $@-t
	mv $@-t $@

$(PROGS) su: ../lib/libshu.a version.o
link_command = $(CC) $(CFLAGS) $(LDFLAGS) -o $@

basename: basename.o long-options.o
	$(link_command) basename.o long-options.o \
	    ../lib/libshu.a version.o $(LIBS)

date: date.o
	$(link_command) date.o ../lib/libshu.a version.o $(LIBS)

dirname: dirname.o long-options.o
	$(link_command) dirname.o long-options.o \
	    ../lib/libshu.a version.o $(LIBS)

echo: echo.o long-options.o
	$(link_command) echo.o long-options.o ../lib/libshu.a version.o $(LIBS)

env: env.o
	$(link_command) env.o ../lib/libshu.a version.o $(LIBS)

expr: expr.o long-options.o
	$(link_command) expr.o long-options.o ../lib/libshu.a version.o $(LIBS)

false: false.sh .version
	rm -f $@ $@-t
	sed -e "s/@VERSION@/`cat .version`/" $(srcdir)/false.sh \
	    > $@-t
	mv $@-t $@
	chmod +x $@

id: id.o
	$(link_command) id.o ../lib/libshu.a version.o $(LIBS)

groups: groups.sh .version
	rm -f $@ $@-t
	sed -e "s,${exec_prefix}/bin,$(bindir),g" \
	    -e "s/@VERSION@/`cat .version`/" $(srcdir)/groups.sh \
	    > $@-t
	mv $@-t $@
	chmod +x $@

logname: logname.o
	$(link_command) logname.o ../lib/libshu.a version.o $(LIBS)

nohup: nohup.sh .version
	rm -f $@ $@-t
	sed -e "s,${exec_prefix}/bin,$(bindir),g" \
	    -e "s/@VERSION@/`cat .version`/" $(srcdir)/nohup.sh \
	    > $@-t
	mv $@-t $@
	chmod +x $@

pathchk: pathchk.o
	$(link_command) pathchk.o ../lib/libshu.a version.o $(LIBS)

printenv: printenv.o
	$(link_command) printenv.o ../lib/libshu.a version.o $(LIBS)

# Link with -lm in case strtod.o needs to get `pow'.
printf: printf.o long-options.o
	$(link_command) printf.o long-options.o ../lib/libshu.a version.o $(LIBS) -lm

sleep: sleep.o
	$(link_command) sleep.o ../lib/libshu.a version.o $(LIBS)

su: su.o
	$(link_command) su.o ../lib/libshu.a version.o $(LIBS)

tee: tee.o
	$(link_command) tee.o ../lib/libshu.a version.o $(LIBS)

test: test.o long-options.o
	$(link_command) test.o long-options.o ../lib/libshu.a version.o $(LIBS)

true: true.sh .version
	rm -f $@ $@-t
	sed -e "s/@VERSION@/`cat .version`/" $(srcdir)/true.sh \
	    > $@-t
	mv $@-t $@
	chmod +x $@

tty: tty.o
	$(link_command) tty.o ../lib/libshu.a version.o $(LIBS)

who: who.o
	$(link_command) who.o ../lib/libshu.a version.o $(LIBS)

whoami: whoami.o
	$(link_command) whoami.o ../lib/libshu.a version.o $(LIBS)

yes: yes.o long-options.o
	$(link_command) yes.o long-options.o ../lib/libshu.a version.o $(LIBS)

nice: nice.o
	$(link_command) nice.o ../lib/libshu.a version.o $(LIBS)

stty: stty.o long-options.o
	$(link_command) stty.o long-options.o ../lib/libshu.a version.o $(LIBS)

uname: uname.o
	$(link_command) uname.o ../lib/libshu.a version.o $(LIBS)

# Special rules for some .o files.

su.o: su.c
	$(CC) -c $(CPPFLAGS) -DSYSLOG_FAILURE -DSYSLOG_SUCCESS $(DEFS) \
	    $(incl) $(CFLAGS) $(srcdir)/su.c

test.o: test.c
	$(CC) -c $(CPPFLAGS) -DTEST_STANDALONE $(DEFS) \
	    $(incl) $(CFLAGS) $(srcdir)/test.c

env.o id.o nice.o pathchk.o stty.o su.o tty.o uname.o who.o: ../lib/getopt.h
expr.o: ../lib/regex.h

# Make all .o files depend on these files even though there are a few
# unnecessary dependencies.
$(OBJECTS): ../config.h system.h version.h
echo.o expr.o printf.o test.o yes.o: long-options.h
basename.o dirname.o stty.o: long-options.h
long-options.o: long-options.h

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
