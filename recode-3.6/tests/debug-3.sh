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
  at_banner="$0: Testing decimal listing of a charset"
  at_dashes=`echo $at_banner | sed s/./=/g`
  echo
  echo "$at_dashes"
  echo "$at_banner"
  echo "$at_dashes"
fi

# Testing decimal listing of a charset
# starting from `lists.m4:3'.
$at_traceon

cat > expect <<'EOF'
ISO-8859-1

  0 NU    16 DL    32 SP    48 0     64 At    80 P     96 '!   112 p
  1 SH    17 D1    33 !     49 1     65 A     81 Q     97 a    113 q
  2 SX    18 D2    34 "     50 2     66 B     82 R     98 b    114 r
  3 EX    19 D3    35 Nb    51 3     67 C     83 S     99 c    115 s
  4 ET    20 D4    36 DO    52 4     68 D     84 T    100 d    116 t
  5 EQ    21 NK    37 %     53 5     69 E     85 U    101 e    117 u
  6 AK    22 SY    38 &     54 6     70 F     86 V    102 f    118 v
  7 BL    23 EB    39 '     55 7     71 G     87 W    103 g    119 w
  8 BS    24 CN    40 (     56 8     72 H     88 X    104 h    120 x
  9 HT    25 EM    41 )     57 9     73 I     89 Y    105 i    121 y
 10 LF    26 SB    42 *     58 :     74 J     90 Z    106 j    122 z
 11 VT    27 EC    43 +     59 ;     75 K     91 <(   107 k    123 (!
 12 FF    28 FS    44 ,     60 <     76 L     92 //   108 l    124 !!
 13 CR    29 GS    45 -     61 =     77 M     93 )>   109 m    125 !)
 14 SO    30 RS    46 .     62 >     78 N     94 '>   110 n    126 '?
 15 SI    31 US    47 /     63 ?     79 O     95 _    111 o    127 DT

128 PA   144 DC   160 NS   176 DG   192 A!   208 D-   224 a!   240 d-
129 HO   145 P1   161 !I   177 +-   193 A'   209 N?   225 a'   241 n?
130 BH   146 P2   162 Ct   178 2S   194 A>   210 O!   226 a>   242 o!
131 NH   147 TS   163 Pd   179 3S   195 A?   211 O'   227 a?   243 o'
132 IN   148 CC   164 Cu   180 ''   196 A:   212 O>   228 a:   244 o>
133 NL   149 MW   165 Ye   181 My   197 AA   213 O?   229 aa   245 o?
134 SA   150 SG   166 BB   182 PI   198 AE   214 O:   230 ae   246 o:
135 ES   151 EG   167 SE   183 .M   199 C,   215 *X   231 c,   247 -:
136 HS   152 SS   168 ':   184 ',   200 E!   216 O/   232 e!   248 o/
137 HJ   153 GC   169 Co   185 1S   201 E'   217 U!   233 e'   249 u!
138 VS   154 SC   170 -a   186 -o   202 E>   218 U'   234 e>   250 u'
139 PD   155 CI   171 <<   187 >>   203 E:   219 U>   235 e:   251 u>
140 PU   156 ST   172 NO   188 14   204 I!   220 U:   236 i!   252 u:
141 RI   157 OC   173 --   189 12   205 I'   221 Y'   237 i'   253 y'
142 S2   158 PM   174 Rg   190 34   206 I>   222 TH   238 i>   254 th
143 S3   159 AC   175 'm   191 ?I   207 I:   223 ss   239 i:   255 y:
EOF

$at_traceoff
test -n "$at_verbose" \
  && echo "$srcdir/lists.m4:44: testing..."
echo lists.m4:44 > at-check-line
test -z "$at_no_redirs" && exec 5>&1 6>&2 1>stdout 2>stderr
$at_traceon
recode -ld latin-1 | cmp -s - expect
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
