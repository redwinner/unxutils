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

rm -rf expect stdout stderr
if test -n "$at_verbose"; then
  at_banner="$0: Testing hexadecimal listing of a charset"
  at_dashes=`echo $at_banner | sed s/./=/g`
  echo
  echo "$at_dashes"
  echo "$at_banner"
  echo "$at_dashes"
fi

# Testing hexadecimal listing of a charset
# starting from `lists.m4:48'.
$at_traceon

cat > expect <<'EOF'
ISO-8859-1

00 NU   10 DL   20 SP   30 0    40 At   50 P    60 '!   70 p
01 SH   11 D1   21 !    31 1    41 A    51 Q    61 a    71 q
02 SX   12 D2   22 "    32 2    42 B    52 R    62 b    72 r
03 EX   13 D3   23 Nb   33 3    43 C    53 S    63 c    73 s
04 ET   14 D4   24 DO   34 4    44 D    54 T    64 d    74 t
05 EQ   15 NK   25 %    35 5    45 E    55 U    65 e    75 u
06 AK   16 SY   26 &    36 6    46 F    56 V    66 f    76 v
07 BL   17 EB   27 '    37 7    47 G    57 W    67 g    77 w
08 BS   18 CN   28 (    38 8    48 H    58 X    68 h    78 x
09 HT   19 EM   29 )    39 9    49 I    59 Y    69 i    79 y
0a LF   1a SB   2a *    3a :    4a J    5a Z    6a j    7a z
0b VT   1b EC   2b +    3b ;    4b K    5b <(   6b k    7b (!
0c FF   1c FS   2c ,    3c <    4c L    5c //   6c l    7c !!
0d CR   1d GS   2d -    3d =    4d M    5d )>   6d m    7d !)
0e SO   1e RS   2e .    3e >    4e N    5e '>   6e n    7e '?
0f SI   1f US   2f /    3f ?    4f O    5f _    6f o    7f DT

80 PA   90 DC   a0 NS   b0 DG   c0 A!   d0 D-   e0 a!   f0 d-
81 HO   91 P1   a1 !I   b1 +-   c1 A'   d1 N?   e1 a'   f1 n?
82 BH   92 P2   a2 Ct   b2 2S   c2 A>   d2 O!   e2 a>   f2 o!
83 NH   93 TS   a3 Pd   b3 3S   c3 A?   d3 O'   e3 a?   f3 o'
84 IN   94 CC   a4 Cu   b4 ''   c4 A:   d4 O>   e4 a:   f4 o>
85 NL   95 MW   a5 Ye   b5 My   c5 AA   d5 O?   e5 aa   f5 o?
86 SA   96 SG   a6 BB   b6 PI   c6 AE   d6 O:   e6 ae   f6 o:
87 ES   97 EG   a7 SE   b7 .M   c7 C,   d7 *X   e7 c,   f7 -:
88 HS   98 SS   a8 ':   b8 ',   c8 E!   d8 O/   e8 e!   f8 o/
89 HJ   99 GC   a9 Co   b9 1S   c9 E'   d9 U!   e9 e'   f9 u!
8a VS   9a SC   aa -a   ba -o   ca E>   da U'   ea e>   fa u'
8b PD   9b CI   ab <<   bb >>   cb E:   db U>   eb e:   fb u>
8c PU   9c ST   ac NO   bc 14   cc I!   dc U:   ec i!   fc u:
8d RI   9d OC   ad --   bd 12   cd I'   dd Y'   ed i'   fd y'
8e S2   9e PM   ae Rg   be 34   ce I>   de TH   ee i>   fe th
8f S3   9f AC   af 'm   bf ?I   cf I:   df ss   ef i:   ff y:
EOF

$at_traceoff
test -n "$at_verbose" \
  && echo "$srcdir/lists.m4:89: testing..."
echo lists.m4:89 > at-check-line
test -z "$at_no_redirs" && exec 5>&1 6>&2 1>stdout 2>stderr
$at_traceon
recode -lh latin-1 | cmp -s - expect
at_status=$?
if test $at_status != 0; then
  test -n "$at_verbose" && test -z "$at_no_redirs" && cat stderr >&6
  exit $at_status
fi
$at_traceoff
if test -z "$at_no_redirs"; then
  exec 1>&5 2>&6
  $at_diff /dev/null stdout || exit 1
  $at_diff /dev/null stderr || exit 1
fi
$at_traceon

$at_traceoff
exit 0
