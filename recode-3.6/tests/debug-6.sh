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
  at_banner="$0: Testing full listing of a charset"
  at_dashes=`echo $at_banner | sed s/./=/g`
  echo
  echo "$at_dashes"
  echo "$at_banner"
  echo "$at_dashes"
fi

# Testing full listing of a charset
# starting from `lists.m4:138'.
$at_traceon

cat > expect <<'EOF'
Dec  Oct Hex   UCS2  Mne  ISO-8859-1

  0  000  00   0000  NU   null (nul)
  1  001  01   0001  SH   start of heading (soh)
  2  002  02   0002  SX   start of text (stx)
  3  003  03   0003  EX   end of text (etx)
  4  004  04   0004  ET   end of transmission (eot)
  5  005  05   0005  EQ   enquiry (enq)
  6  006  06   0006  AK   acknowledge (ack)
  7  007  07   0007  BL   bell (bel)
  8  010  08   0008  BS   backspace (bs)
  9  011  09   0009  HT   character tabulation (ht)
 10  012  0a   000A  LF   line feed (lf)
 11  013  0b   000B  VT   line tabulation (vt)
 12  014  0c   000C  FF   form feed (ff)
 13  015  0d   000D  CR   carriage return (cr)
 14  016  0e   000E  SO   shift out (so)
 15  017  0f   000F  SI   shift in (si)
 16  020  10   0010  DL   datalink escape (dle)
 17  021  11   0011  D1   device control one (dc1)
 18  022  12   0012  D2   device control two (dc2)
 19  023  13   0013  D3   device control three (dc3)
 20  024  14   0014  D4   device control four (dc4)
 21  025  15   0015  NK   negative acknowledge (nak)
 22  026  16   0016  SY   syncronous idle (syn)
 23  027  17   0017  EB   end of transmission block (etb)
 24  030  18   0018  CN   cancel (can)
 25  031  19   0019  EM   end of medium (em)
 26  032  1a   001A  SB   substitute (sub)
 27  033  1b   001B  EC   escape (esc)
 28  034  1c   001C  FS   file separator (is4)
 29  035  1d   001D  GS   group separator (is3)
 30  036  1e   001E  RS   record separator (is2)
 31  037  1f   001F  US   unit separator (is1)
 32  040  20   0020  SP   space
 33  041  21   0021  !    exclamation mark
 34  042  22   0022  "    quotation mark
 35  043  23   0023  Nb   number sign
 36  044  24   0024  DO   dollar sign
 37  045  25   0025  %    percent sign
 38  046  26   0026  &    ampersand
 39  047  27   0027  '    apostrophe
 40  050  28   0028  (    left parenthesis
 41  051  29   0029  )    right parenthesis
 42  052  2a   002A  *    asterisk
 43  053  2b   002B  +    plus sign
 44  054  2c   002C  ,    comma
 45  055  2d   002D  -    hyphen-minus
 46  056  2e   002E  .    full stop
 47  057  2f   002F  /    solidus
 48  060  30   0030  0    digit zero
 49  061  31   0031  1    digit one
 50  062  32   0032  2    digit two
 51  063  33   0033  3    digit three
 52  064  34   0034  4    digit four
 53  065  35   0035  5    digit five
 54  066  36   0036  6    digit six
 55  067  37   0037  7    digit seven
 56  070  38   0038  8    digit eight
 57  071  39   0039  9    digit nine
 58  072  3a   003A  :    colon
 59  073  3b   003B  ;    semicolon
 60  074  3c   003C  <    less-than sign
 61  075  3d   003D  =    equals sign
 62  076  3e   003E  >    greater-than sign
 63  077  3f   003F  ?    question mark
 64  100  40   0040  At   commercial at
 65  101  41   0041  A    latin capital letter a
 66  102  42   0042  B    latin capital letter b
 67  103  43   0043  C    latin capital letter c
 68  104  44   0044  D    latin capital letter d
 69  105  45   0045  E    latin capital letter e
 70  106  46   0046  F    latin capital letter f
 71  107  47   0047  G    latin capital letter g
 72  110  48   0048  H    latin capital letter h
 73  111  49   0049  I    latin capital letter i
 74  112  4a   004A  J    latin capital letter j
 75  113  4b   004B  K    latin capital letter k
 76  114  4c   004C  L    latin capital letter l
 77  115  4d   004D  M    latin capital letter m
 78  116  4e   004E  N    latin capital letter n
 79  117  4f   004F  O    latin capital letter o
 80  120  50   0050  P    latin capital letter p
 81  121  51   0051  Q    latin capital letter q
 82  122  52   0052  R    latin capital letter r
 83  123  53   0053  S    latin capital letter s
 84  124  54   0054  T    latin capital letter t
 85  125  55   0055  U    latin capital letter u
 86  126  56   0056  V    latin capital letter v
 87  127  57   0057  W    latin capital letter w
 88  130  58   0058  X    latin capital letter x
 89  131  59   0059  Y    latin capital letter y
 90  132  5a   005A  Z    latin capital letter z
 91  133  5b   005B  <(   left square bracket
 92  134  5c   005C  //   reverse solidus
 93  135  5d   005D  )>   right square bracket
 94  136  5e   005E  '>   circumflex accent
 95  137  5f   005F  _    low line
 96  140  60   0060  '!   grave accent
 97  141  61   0061  a    latin small letter a
 98  142  62   0062  b    latin small letter b
 99  143  63   0063  c    latin small letter c
100  144  64   0064  d    latin small letter d
101  145  65   0065  e    latin small letter e
102  146  66   0066  f    latin small letter f
103  147  67   0067  g    latin small letter g
104  150  68   0068  h    latin small letter h
105  151  69   0069  i    latin small letter i
106  152  6a   006A  j    latin small letter j
107  153  6b   006B  k    latin small letter k
108  154  6c   006C  l    latin small letter l
109  155  6d   006D  m    latin small letter m
110  156  6e   006E  n    latin small letter n
111  157  6f   006F  o    latin small letter o
112  160  70   0070  p    latin small letter p
113  161  71   0071  q    latin small letter q
114  162  72   0072  r    latin small letter r
115  163  73   0073  s    latin small letter s
116  164  74   0074  t    latin small letter t
117  165  75   0075  u    latin small letter u
118  166  76   0076  v    latin small letter v
119  167  77   0077  w    latin small letter w
120  170  78   0078  x    latin small letter x
121  171  79   0079  y    latin small letter y
122  172  7a   007A  z    latin small letter z
123  173  7b   007B  (!   left curly bracket
124  174  7c   007C  !!   vertical line
125  175  7d   007D  !)   right curly bracket
126  176  7e   007E  '?   tilde
127  177  7f   007F  DT   delete (del)
128  200  80   0080  PA   padding character (pad)
129  201  81   0081  HO   high octet preset (hop)
130  202  82   0082  BH   break permitted here (bph)
131  203  83   0083  NH   no break here (nbh)
132  204  84   0084  IN   index (ind)
133  205  85   0085  NL   next line (nel)
134  206  86   0086  SA   start of selected area (ssa)
135  207  87   0087  ES   end of selected area (esa)
136  210  88   0088  HS   character tabulation set (hts)
137  211  89   0089  HJ   character tabulation with justification (htj)
138  212  8a   008A  VS   line tabulation set (vts)
139  213  8b   008B  PD   partial line forward (pld)
140  214  8c   008C  PU   partial line backward (plu)
141  215  8d   008D  RI   reverse line feed (ri)
142  216  8e   008E  S2   single-shift two (ss2)
143  217  8f   008F  S3   single-shift three (ss3)
144  220  90   0090  DC   device control string (dcs)
145  221  91   0091  P1   private use one (pu1)
146  222  92   0092  P2   private use two (pu2)
147  223  93   0093  TS   set transmit state (sts)
148  224  94   0094  CC   cancel character (cch)
149  225  95   0095  MW   message waiting (mw)
150  226  96   0096  SG   start of guarded area (spa)
151  227  97   0097  EG   end of guarded area (epa)
152  230  98   0098  SS   start of string (sos)
153  231  99   0099  GC   single graphic character introducer (sgci)
154  232  9a   009A  SC   single character introducer (sci)
155  233  9b   009B  CI   control sequence introducer (csi)
156  234  9c   009C  ST   string terminator (st)
157  235  9d   009D  OC   operating system command (osc)
158  236  9e   009E  PM   privacy message (pm)
159  237  9f   009F  AC   application program command (apc)
160  240  a0   00A0  NS   no-break space
161  241  a1   00A1  !I   inverted exclamation mark
162  242  a2   00A2  Ct   cent sign
163  243  a3   00A3  Pd   pound sign
164  244  a4   00A4  Cu   currency sign
165  245  a5   00A5  Ye   yen sign
166  246  a6   00A6  BB   broken bar
167  247  a7   00A7  SE   section sign
168  250  a8   00A8  ':   diaeresis
169  251  a9   00A9  Co   copyright sign
170  252  aa   00AA  -a   feminine ordinal indicator
171  253  ab   00AB  <<   left-pointing double angle quotation mark
172  254  ac   00AC  NO   not sign
173  255  ad   00AD  --   soft hyphen
174  256  ae   00AE  Rg   registered sign
175  257  af   00AF  'm   macron
176  260  b0   00B0  DG   degree sign
177  261  b1   00B1  +-   plus-minus sign
178  262  b2   00B2  2S   superscript two
179  263  b3   00B3  3S   superscript three
180  264  b4   00B4  ''   acute accent
181  265  b5   00B5  My   micro sign
182  266  b6   00B6  PI   pilcrow sign
183  267  b7   00B7  .M   middle dot
184  270  b8   00B8  ',   cedilla
185  271  b9   00B9  1S   superscript one
186  272  ba   00BA  -o   masculine ordinal indicator
187  273  bb   00BB  >>   right-pointing double angle quotation mark
188  274  bc   00BC  14   vulgar fraction one quarter
189  275  bd   00BD  12   vulgar fraction one half
190  276  be   00BE  34   vulgar fraction three quarters
191  277  bf   00BF  ?I   inverted question mark
192  300  c0   00C0  A!   latin capital letter a with grave
193  301  c1   00C1  A'   latin capital letter a with acute
194  302  c2   00C2  A>   latin capital letter a with circumflex
195  303  c3   00C3  A?   latin capital letter a with tilde
196  304  c4   00C4  A:   latin capital letter a with diaeresis
197  305  c5   00C5  AA   latin capital letter a with ring above
198  306  c6   00C6  AE   latin capital letter ae
199  307  c7   00C7  C,   latin capital letter c with cedilla
200  310  c8   00C8  E!   latin capital letter e with grave
201  311  c9   00C9  E'   latin capital letter e with acute
202  312  ca   00CA  E>   latin capital letter e with circumflex
203  313  cb   00CB  E:   latin capital letter e with diaeresis
204  314  cc   00CC  I!   latin capital letter i with grave
205  315  cd   00CD  I'   latin capital letter i with acute
206  316  ce   00CE  I>   latin capital letter i with circumflex
207  317  cf   00CF  I:   latin capital letter i with diaeresis
208  320  d0   00D0  D-   latin capital letter eth (icelandic)
209  321  d1   00D1  N?   latin capital letter n with tilde
210  322  d2   00D2  O!   latin capital letter o with grave
211  323  d3   00D3  O'   latin capital letter o with acute
212  324  d4   00D4  O>   latin capital letter o with circumflex
213  325  d5   00D5  O?   latin capital letter o with tilde
214  326  d6   00D6  O:   latin capital letter o with diaeresis
215  327  d7   00D7  *X   multiplication sign
216  330  d8   00D8  O/   latin capital letter o with stroke
217  331  d9   00D9  U!   latin capital letter u with grave
218  332  da   00DA  U'   latin capital letter u with acute
219  333  db   00DB  U>   latin capital letter u with circumflex
220  334  dc   00DC  U:   latin capital letter u with diaeresis
221  335  dd   00DD  Y'   latin capital letter y with acute
222  336  de   00DE  TH   latin capital letter thorn (icelandic)
223  337  df   00DF  ss   latin small letter sharp s (german)
224  340  e0   00E0  a!   latin small letter a with grave
225  341  e1   00E1  a'   latin small letter a with acute
226  342  e2   00E2  a>   latin small letter a with circumflex
227  343  e3   00E3  a?   latin small letter a with tilde
228  344  e4   00E4  a:   latin small letter a with diaeresis
229  345  e5   00E5  aa   latin small letter a with ring above
230  346  e6   00E6  ae   latin small letter ae
231  347  e7   00E7  c,   latin small letter c with cedilla
232  350  e8   00E8  e!   latin small letter e with grave
233  351  e9   00E9  e'   latin small letter e with acute
234  352  ea   00EA  e>   latin small letter e with circumflex
235  353  eb   00EB  e:   latin small letter e with diaeresis
236  354  ec   00EC  i!   latin small letter i with grave
237  355  ed   00ED  i'   latin small letter i with acute
238  356  ee   00EE  i>   latin small letter i with circumflex
239  357  ef   00EF  i:   latin small letter i with diaeresis
240  360  f0   00F0  d-   latin small letter eth (icelandic)
241  361  f1   00F1  n?   latin small letter n with tilde
242  362  f2   00F2  o!   latin small letter o with grave
243  363  f3   00F3  o'   latin small letter o with acute
244  364  f4   00F4  o>   latin small letter o with circumflex
245  365  f5   00F5  o?   latin small letter o with tilde
246  366  f6   00F6  o:   latin small letter o with diaeresis
247  367  f7   00F7  -:   division sign
248  370  f8   00F8  o/   latin small letter o with stroke
249  371  f9   00F9  u!   latin small letter u with grave
250  372  fa   00FA  u'   latin small letter u with acute
251  373  fb   00FB  u>   latin small letter u with circumflex
252  374  fc   00FC  u:   latin small letter u with diaeresis
253  375  fd   00FD  y'   latin small letter y with acute
254  376  fe   00FE  th   latin small letter thorn (icelandic)
255  377  ff   00FF  y:   latin small letter y with diaeresis
EOF

$at_traceoff
test -n "$at_verbose" \
  && echo "$srcdir/lists.m4:402: testing..."
echo lists.m4:402 > at-check-line
test -z "$at_no_redirs" && exec 5>&1 6>&2 1>stdout 2>stderr
$at_traceon
recode -lf latin-1 | cmp -s - expect
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
