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
  at_banner="$0: Testing testing and counting"
  at_dashes=`echo $at_banner | sed s/./=/g`
  echo
  echo "$at_dashes"
  echo "$at_banner"
  echo "$at_dashes"
fi

# Testing testing and counting
# starting from `testdump.m4:3'.
$at_traceon

$at_traceoff
test -n "$at_verbose" \
  && echo "$srcdir/testdump.m4:11: testing..."
echo testdump.m4:11 > at-check-line
test -z "$at_no_redirs" && exec 5>&1 6>&2 1>stdout 2>stderr
$at_traceon
recode test7..x,us..count < /dev/null
at_status=$?
if test $at_status != 0; then
  test -n "$at_verbose" && test -z "$at_no_redirs" && cat stderr >&6
  exit $at_status
fi
$at_traceoff
if test -z "$at_no_redirs"; then
  exec 1>&5 2>&6
  echo $at_n " 11  000A LF   117  0020 SP   127  002C ,    152  0030 0     24  0031 1
 24  0032 2     24  0033 3     24  0034 4     24  0035 5     24  0036 6
 24  0037 7      8  0038 8      8  0039 9      8  0041 A      8  0042 B
  8  0043 C      8  0044 D      8  0045 E      8  0046 F    128  0078 x
$at_c" | $at_diff - stdout || exit 1
  $at_diff /dev/null stderr || exit 1
fi
$at_traceon

$at_traceoff
test -n "$at_verbose" \
  && echo "$srcdir/testdump.m4:18: testing..."
echo testdump.m4:18 > at-check-line
test -z "$at_no_redirs" && exec 5>&1 6>&2 1>stdout 2>stderr
$at_traceon
recode test8..x,us..count < /dev/null
at_status=$?
if test $at_status != 0; then
  test -n "$at_verbose" && test -z "$at_no_redirs" && cat stderr >&6
  exit $at_status
fi
$at_traceoff
if test -z "$at_no_redirs"; then
  exec 1>&5 2>&6
  echo $at_n " 22  000A LF   234  0020 SP   255  002C ,    288  0030 0     32  0031 1
 32  0032 2     32  0033 3     32  0034 4     32  0035 5     32  0036 6
 32  0037 7     32  0038 8     32  0039 9     32  0041 A     32  0042 B
 32  0043 C     32  0044 D     32  0045 E     32  0046 F    256  0078 x
$at_c" | $at_diff - stdout || exit 1
  $at_diff /dev/null stderr || exit 1
fi
$at_traceon

$at_traceoff
test -n "$at_verbose" \
  && echo "$srcdir/testdump.m4:26: testing..."
echo testdump.m4:26 > at-check-line
test -z "$at_no_redirs" && exec 5>&1 6>&2 1>stdout 2>stderr
$at_traceon
recode test15..x2,us..count < /dev/null
at_status=$?
if test $at_status != 0; then
  test -n "$at_verbose" && test -z "$at_no_redirs" && cat stderr >&6
  exit $at_status
fi
$at_traceoff
if test -z "$at_no_redirs"; then
  exec 1>&5 2>&6
  echo $at_n " 8064  000A LF   56445  0020 SP   64508  002C ,    80765  0030 0
16256  0031 1    16256  0032 2    16256  0033 3    16256  0034 4
16256  0035 5    16256  0036 6    16256  0037 7    16256  0038 8
16256  0039 9    16256  0041 A    16256  0042 B    16000  0043 C
14975  0044 D    15999  0045 E    15990  0046 F    64509  0078 x
$at_c" | $at_diff - stdout || exit 1
  $at_diff /dev/null stderr || exit 1
fi
$at_traceon

$at_traceoff
test -n "$at_verbose" \
  && echo "$srcdir/testdump.m4:34: testing..."
echo testdump.m4:34 > at-check-line
test -z "$at_no_redirs" && exec 5>&1 6>&2 1>stdout 2>stderr
$at_traceon
recode test16..x2,us..count < /dev/null
at_status=$?
if test $at_status != 0; then
  test -n "$at_verbose" && test -z "$at_no_redirs" && cat stderr >&6
  exit $at_status
fi
$at_traceoff
if test -z "$at_no_redirs"; then
  exec 1>&5 2>&6
  echo $at_n " 8192  000A LF   57344  0020 SP   65535  002C ,    81920  0030 0
16384  0031 1    16384  0032 2    16384  0033 3    16384  0034 4
16384  0035 5    16384  0036 6    16384  0037 7    16384  0038 8
16384  0039 9    16384  0041 A    16384  0042 B    16384  0043 C
16384  0044 D    16384  0045 E    16384  0046 F    65536  0078 x
$at_c" | $at_diff - stdout || exit 1
  $at_diff /dev/null stderr || exit 1
fi
$at_traceon

$at_traceoff
exit 0
