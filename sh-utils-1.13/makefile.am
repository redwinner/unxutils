## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = gnits 1.1k

SUBDIRS = lib intl src doc man po tests
EXTRA_DIST = ChangeLog.0

install-root:
	cd src && $(MAKE) $@

maintainer-check:

# Tag before making distribution.  Also, don't make a distribution if
# checks fail.  Also, make sure the NEWS file is up-to-date.
cvs-dist: maintainer-check
	pkg=`echo "@PACKAGE@" | tr a-z A-Z`; \
	ver=`echo "@VERSION@" | sed 's/\./_/g'`; \
	tag="$$pkg-$$ver"; \
	echo tag=$$tag; \
	if cvs -n log -h README| grep -e $$tag > /dev/null; then \
	  echo "VERSION not new; not releasing" 1>&2; \
	  exit 1; \
	else :; \
	fi; \
	cvs tag -c $$tag
	$(MAKE) dist
