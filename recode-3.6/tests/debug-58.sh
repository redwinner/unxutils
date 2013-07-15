#!/bin/sh
# This debugging script has been automatically generated from `make check'.
# Call it with `--help' to get a quick usage summary.

at_package=recode
at_version=3.6
at_verbose=1
at_n=-n
at_c=''
test -f atlocal && . ./atlocal

# LC_MESSAGES is always shadowed by LC_ALL.  Here are the only cases:
# - GNU:       LANGUAGE -> LC_ALL -> LC_MESSAGES -> LANG
# - POSIX:     LC_ALL -> LC_MESSAGES -> LANG
# - XPG4:      LC_ALL -> LANG
# - SysV/XPG2: LANG
export LANGUAGE; LANGUAGE=C
export LANG; LANG=C
export LC_ALL; LC_ALL=C

srcdir=.
top_srcdir=..
# Use absolute file notations, as the test might change directories.
at_srcdir=`cd . && pwd`
at_top_srcdir=`cd .. && pwd`
if test -n "../src"; then
  export PATH; PATH=`pwd`:`cd ../src && pwd`:$PATH
else
  export PATH; PATH=`pwd`:$PATH
fi

at_usage="Usage: $0 OPTION...

  -e  Abort the full suite and inhibit normal clean up if a test fails
  -n  Do not redirect stdout and stderr and do not test their contents
  -s  Inhibit verbosity while generating or executing debugging scripts
  -v  Force more detailed output, default for debugging scripts unless -s
  -x  Have the shell to trace command execution; also implies option -n"

while test $# -gt 0; do
  case "$1" in
    --help) echo "$at_usage"; exit 0 ;;
    --version) echo "$0 ($at_package) $at_version"; exit 0 ;;
    -e) at_stop_on_error=1; shift ;;
    -n) at_no_redirs=1; shift ;;
    -s) at_verbose=; at_silent=1; shift ;;
    -v) at_verbose=1; at_silent=; shift ;;
    -x) at_traceon='set -vx'; at_traceoff='set +vx'; at_no_redirs=1; shift ;;
    *) echo 1>&2 "Try \`$0 --help' for more information."; exit 1 ;;
  esac
done

# In the testing suite, we only want to know if the test succeeded or failed.
# But in debugging scripts, we want more information, so we prefer `diff -u'
# to the silent `cmp', even if it may happen that we compare binary files.
# Option `-u' might be less portable, so either change it or use GNU `diff'.

if test -n "$at_verbose"; then
  at_diff='diff -u'
else
  if test -n "$COMSPEC$ComSpec"; then
    at_diff='diff -u'
  else
    at_diff='cmp -s'
  fi
fi

# Each generated debugging script, containing a single test group, cleans
# up files at the beginning only, not at the end.  This is so we can repeat
# the script many times and browse left over files.  To cope with such left
# over files, the full test suite cleans up both before and after test groups.

rm -rf stdout stderr
if test -n "$at_verbose"; then
  at_banner="$0: Testing block of lines to UTF-8"
  at_dashes=`echo $at_banner | sed s/./=/g`
  echo
  echo "$at_dashes"
  echo "$at_banner"
  echo "$at_dashes"
fi

# Testing block of lines to UTF-8
# starting from `utf8.m4:3'.
$at_traceon

$at_traceoff
test -n "$at_verbose" \
  && echo "$srcdir/utf8.m4:25: testing..."
echo utf8.m4:25 > at-check-line
test -z "$at_no_redirs" && exec 5>&1 6>&2 1>stdout 2>stderr
$at_traceon
recode l1/qp..u8/x < $at_srcdir/quoted-data

at_status=$?
if test $at_status != 0; then
  test -n "$at_verbose" && test -z "$at_no_redirs" && cat stderr >&6
  exit $at_status
fi
$at_traceoff
if test -z "$at_no_redirs"; then
  exec 1>&5 2>&6
  echo $at_n "0x44, 0x65, 0x61, 0x72, 0x20, 0xC3, 0x9E, 0x6F, 0x72, 0x76, 0x61, 0x72,
0x64, 0xC3, 0xB0, 0x75, 0x72, 0x2C, 0x0A, 0x0A, 0xC2, 0xAB, 0x20, 0x4F,
0xC3, 0xB9, 0x20, 0x71, 0x75, 0x27, 0x69, 0x6C, 0x20, 0x72, 0xC3, 0xA9,
0x73, 0x69, 0x64, 0x65, 0x2C, 0x20, 0xC3, 0xA0, 0x20, 0x4E, 0xC3, 0xAE,
0x6D, 0x65, 0x73, 0x20, 0x6F, 0x75, 0x20, 0x6D, 0xC3, 0xAA, 0x6D, 0x65,
0x20, 0x43, 0x61, 0x70, 0x68, 0x61, 0x72, 0x6E, 0x61, 0xC3, 0xBC, 0x6D,
0x2C, 0x20, 0x74, 0x6F, 0x75, 0x74, 0x20, 0x46, 0x72, 0x61, 0x6E, 0xC3,
0xA7, 0x61, 0x69, 0x73, 0x20, 0x69, 0x6E, 0x73, 0x63, 0x72, 0x69, 0x74,
0x0A, 0x61, 0x75, 0x20, 0x72, 0xC3, 0xB4, 0x6C, 0x65, 0x20, 0x70, 0x61,
0x79, 0x65, 0x72, 0x61, 0x20, 0x73, 0x6F, 0x6E, 0x20, 0x64, 0xC3, 0xBB,
0x20, 0x64, 0xC3, 0xA8, 0x73, 0x20, 0x61, 0x76, 0x61, 0x6E, 0x74, 0x20,
0x4E, 0x6F, 0xC3, 0xAB, 0x6C, 0x2C, 0x20, 0x71, 0x75, 0x27, 0x69, 0x6C,
0x20, 0x73, 0x6F, 0x69, 0x74, 0x20, 0x6E, 0x61, 0xC3, 0xAF, 0x66, 0x20,
0x6F, 0x75, 0x20, 0x72, 0xC3, 0xA2, 0x6C, 0x65, 0x75, 0x72, 0x2E, 0x20,
0xC2, 0xBB, 0x0A, 0x49, 0x6E, 0x63, 0x6F, 0x6D, 0x70, 0x6C, 0x65, 0x74,
0x65, 0x20, 0x6C, 0x61, 0x73, 0x74, 0x20, 0x6C, 0x69, 0x6E, 0x65
$at_c" | $at_diff - stdout || exit 1
  $at_diff /dev/null stderr || exit 1
fi
$at_traceon

$at_traceoff
exit 0
