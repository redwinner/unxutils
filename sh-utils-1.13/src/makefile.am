## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = ansi2knr

EXTRAdir = $(bindir)
EXTRA_PROGRAMS = chroot nice stty uname uptime
EXTRA_SCRIPTS = nohup

bin_SCRIPTS = false groups true @OPTIONAL_BIN_ZCRIPTS@

bin_PROGRAMS = basename date dirname echo env expr factor \
  hostname id logname pathchk printenv printf pwd seq sleep su tee test \
  tty users who whoami yes @OPTIONAL_BIN_PROGS@

datadir = $(prefix)/@DATADIRNAME@
localedir = $(datadir)/locale

noinst_HEADERS = system.h
EXTRA_DIST = ansi2knr.1 ansi2knr.c \
  groups.sh nohup.sh false.sh true.sh who-users.c
DISTCLEANFILES = $(SCRIPTS)
MOSTLYCLEANFILES = who.c users.c uptime.c

INCLUDES = -I.. -I$(srcdir) -I$(top_srcdir)/lib -I../intl
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
LDADD = @INTLLIBS@ ../lib/libsu.a

## For sqrt
factor_LDADD = $(LDADD) @SQRT_LIBM@
factor_DEPENDENCIES = $(LDADD)

## If necessary, add -lm to resolve use of pow in lib/strtod.c.
printf_LDADD = $(LDADD) @POW_LIBM@
printf_DEPENDENCIES = $(LDADD)

## For floor, rint, modf
seq_LDADD = $(LDADD) @SEQ_LIBM@
seq_DEPENDENCIES = $(LDADD)

uptime_LDADD = $(LDADD) @GETLOADAVG_LIBS@
uptime_DEPENDENCIES = $(LDADD)

$(PROGRAMS): ../lib/libsu.a

$(SCRIPTS): Makefile

SUFFIXES = .sh

.sh:
	rm -f $@ $@-t
	sed \
	  -e 's!@''bindir''@!@bindir@!' \
	  -e 's/@''GNU_PACKAGE''@/@GNU_PACKAGE@/' \
	  -e 's/@''VERSION''@/@VERSION@/' $< > $@-t
	chmod +x $@-t
	mv $@-t $@

BUILT_SOURCES = who.c users.c uptime.c

who.c: who-users.c
	rm -f $@
	echo "#define WHO 1" > $@-t
	cat $(srcdir)/who-users.c >> $@-t && mv $@-t $@ && chmod a-w $@

users.c: who-users.c
	rm -f $@
	echo "#define USERS 1" > $@-t
	cat $(srcdir)/who-users.c >> $@-t && mv $@-t $@ && chmod a-w $@

uptime.c: who-users.c
	rm -f $@
	echo "#define UPTIME 1" > $@-t
	cat $(srcdir)/who-users.c >> $@-t && mv $@-t $@ && chmod a-w $@

install-exec-local: install-binPROGRAMS
	@if chown root $(bindir)/`echo su|sed '$(transform)'` 2>&1 /dev/null \
	    && chmod u+s $(bindir)/`echo su|sed '$(transform)'`; then \
	  : ; \
	else \
	  echo "WARNING: insufficient access; not installing su"; \
	  echo "NOTE: to install su, run 'make install-root' as root"; \
	  rm -f $(bindir)/`echo su|sed '$(transform)'`; \
	fi

installed_su = $(bindir)/`echo su|sed '$(transform)'`
install-root: su
	$(INSTALL_PROGRAM) su $(installed_su)
	chown root $(installed_su)
	chmod u+s $(installed_su)
