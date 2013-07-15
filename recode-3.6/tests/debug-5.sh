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
  at_banner="$0: Testing octal listing of a charset"
  at_dashes=`echo $at_banner | sed s/./=/g`
  echo
  echo "$at_dashes"
  echo "$at_banner"
  echo "$at_dashes"
fi

# Testing octal listing of a charset
# starting from `lists.m4:93'.
$at_traceon

cat > expect <<'EOF'
ISO-8859-1

000 NU   020 DL   040 SP   060 0    100 At   120 P    140 '!   160 p
001 SH   021 D1   041 !    061 1    101 A    121 Q    141 a    161 q
002 SX   022 D2   042 "    062 2    102 B    122 R    142 b    162 r
003 EX   023 D3   043 Nb   063 3    103 C    123 S    143 c    163 s
004 ET   024 D4   044 DO   064 4    104 D    124 T    144 d    164 t
005 EQ   025 NK   045 %    065 5    105 E    125 U    145 e    165 u
006 AK   026 SY   046 &    066 6    106 F    126 V    146 f    166 v
007 BL   027 EB   047 '    067 7    107 G    127 W    147 g    167 w
010 BS   030 CN   050 (    070 8    110 H    130 X    150 h    170 x
011 HT   031 EM   051 )    071 9    111 I    131 Y    151 i    171 y
012 LF   032 SB   052 *    072 :    112 J    132 Z    152 j    172 z
013 VT   033 EC   053 +    073 ;    113 K    133 <(   153 k    173 (!
014 FF   034 FS   054 ,    074 <    114 L    134 //   154 l    174 !!
015 CR   035 GS   055 -    075 =    115 M    135 )>   155 m    175 !)
016 SO   036 RS   056 .    076 >    116 N    136 '>   156 n    176 '?
017 SI   037 US   057 /    077 ?    117 O    137 _    157 o    177 DT

200 PA   220 DC   240 NS   260 DG   300 A!   320 D-   340 a!   360 d-
201 HO   221 P1   241 !I   261 +-   301 A'   321 N?   341 a'   361 n?
202 BH   222 P2   242 Ct   262 2S   302 A>   322 O!   342 a>   362 o!
203 NH   223 TS   243 Pd   263 3S   303 A?   323 O'   343 a?   363 o'
204 IN   224 CC   244 Cu   264 ''   304 A:   324 O>   344 a:   364 o>
205 NL   225 MW   245 Ye   265 My   305 AA   325 O?   345 aa   365 o?
206 SA   226 SG   246 BB   266 PI   306 AE   326 O:   346 ae   366 o:
207 ES   227 EG   247 SE   267 .M   307 C,   327 *X   347 c,   367 -:
210 HS   230 SS   250 ':   270 ',   310 E!   330 O/   350 e!   370 o/
211 HJ   231 GC   251 Co   271 1S   311 E'   331 U!   351 e'   371 u!
212 VS   232 SC   252 -a   272 -o   312 E>   332 U'   352 e>   372 u'
213 PD   233 CI   253 <<   273 >>   313 E:   333 U>   353 e:   373 u>
214 PU   234 ST   254 NO   274 14   314 I!   334 U:   354 i!   374 u:
215 RI   235 OC   255 --   275 12   315 I'   335 Y'   355 i'   375 y'
216 S2   236 PM   256 Rg   276 34   316 I>   336 TH   356 i>   376 th
217 S3   237 AC   257 'm   277 ?I   317 I:   337 ss   357 i:   377 y:
EOF

$at_traceoff
test -n "$at_verbose" \
  && echo "$srcdir/lists.m4:134: testing..."
echo lists.m4:134 > at-check-line
test -z "$at_no_redirs" && exec 5>&1 6>&2 1>stdout 2>stderr
$at_traceon
recode -lo latin-1 | cmp -s - expect
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
