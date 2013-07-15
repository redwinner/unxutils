"/dev/stderr"
"foo"
"usage:
# Arnold Robbins, arnold@gnu.org, Public Domain
# May 1993
# Public Domain
# nextfile --- skip remaining records in current file
# this should be read in before the "main" awk program
%s\n",
'BEGIN
((getline
(ARGC
(E.g.,
(See
(d.c.).
(i.e.
(see
(such
(the
), it is not special.
).  For example:
).  Interval expressions are allowed.
).  Otherwise, it will be treated as an error, since the POSIX standard specifies that
+---------+---------+--------+---------+
, Prev:
, Up:
, a program that you can use to select particular records in a file and perform operations upon them.
, and
, and also see
, but instead of adding, it subtracts.  It decrements
, for a description of the line continuation mechanism in
, for more information about the
, for more information.
, in which case the newline is ignored.
, supply a leading
, the result will always contain a decimal point.  For
, there is no such limit.  You can open as many pipelines as the underlying operating system will permit.
, trailing zeros are not removed from the result.
, with a total of four significant figures of which three follow the decimal point.  The
--------         ----------          ---------------
--------+----------------------------------------------
--lint
--posix
--re-interval
--source
--traditional
--version
-| Record = record
-| aardvark     555-5553     1200/300          B
-| alpo-net     555-3412     2400/1200/300     A
-| barfly       555-7685     1200/300          A
-| bites        555-1675     2400/1200/300     A
-| camelot      555-0542     300               C
-| core         555-2912     1200/300          C
-| fooey        555-1234     2400/1200/300     B
-| foot         555-6699     1200/300          B
-| macfoo       555-6480     1200/300          A
-| sabafoo      555-2127     1200/300          C
-| sdace        555-3430     2400/1200/300     A
.  Leading and trailing matches of
.  See
.  The
.  The pipeline to the command stays open until the
.  This applies to both the statements within the action part of a rule (the usual case), and to the rule statements.
.  You might want it to be in a different directory on your system.
...
/dev/stderr
/dev/stdin
/dev/stdout
/dev/user
/etc/passwd
/foo/
/tmp/ig.s.$$
1200/300
1993
2  Who are you?  The new number two!
2400/1200/300
555-1234
555-2127
555-3412
555-5553
555-6480
555-6699
; for example,
<1>:
<2>:
>                 
>                       print $0; print NF }'
>        for (i = 0; i < ARGC; i++)
>      }' inventory-shipped BBS-list
@include
A Library of
A More Complex Example
A Simple Stream Editor
A function for handling data file transitions.
A function for processing command line arguments.
A function to join an array into a string.
A function to turn a date into a timestamp.
A less simple one-line example with two rules.
A program to produce a word usage count.
ARGC
ARGC and ARGV
ARGV
ARGV[1]
ARGV[2]
ARGV[i] = ""
ASCII
AWKPATH
AWKPATH Variable
About this help file
According to POSIX,
Acknowledgements
Action Overview
Actions
Actions Summary
Adding Code
Adding New Features
Additional Regexp Operators Only in
Additions
After
Alarm Program
Allow interval expressions in regexps, even if
Also
Alternate instructions for VMS POSIX.
Although
Amiga Installation
An Alarm Clock Program
An Easy Way to Use Library Functions
An example function definition and what it does.
Another looping statement, that provides initialization and increment clauses.
Any expression can be used as a pattern.
Arguments
Arithmetic Operators
Arithmetic Ops
Arnold
Array
Array Example
Array Intro
Arrays
Arrays Summary
Arrays in
Assert Function
Assigning Elements
Assigning Variables on the Command Line
Assignment Expressions
Assignment Ops
Assignment Options
Atari
Atari Compiling
Atari Installation
Auto-set
BBS-list
BEGIN
BEGIN    \
BEGIN {
BEGIN/END
Basic Field Splitting
Basic Printf
Before
Boolean Expressions
Boolean Ops
Bourne-Again
Break Statement
Brief history of the GNU project and this Info file.
Buffering
Bugs
Built-in
Built-in Functions
Built-in Functions Summary
Built-in Functions for Input/Output
Built-in Functions for String Manipulation
Built-in Summary
Built-in Variables
Built-in Variables that Convey Information
Built-in numeric and string functions.
Built-in variables that you change to control
CONVFMT
Calling Built-in
Calls
Case-sensitivity
Case-sensitivity in Matching
Caution:
Caveats
Changing
Changing Fields
Changing the Contents of a Field
Changing the value of a variable or a field.
Character
Clones
Close Files And Pipes
Closing Input and Output Files and Pipes
Closing Input and Output Files and Pipes.
Code
Combining comparison expressions using boolean operators
Command
Command Line Field Separator
Command Line Options
Command Line Summary
Command line options and their meanings.
Comments
Comparison
Compatibility Mode
Compilation
Compiling
Compiling and Installing
Complex
Computed Regexps
Concatenation
Conditional Exp
Conditional Expressions
Conditional expressions select between two subexpressions under control of a third subexpression.
Configuration Philosophy
Constant Size
Constants
Contents
Continue Statement
Control Flow Summary
Control Letters
Control Statements in Actions
Control-d
Controlling Numeric Output With
Controlling how data is split into records.
Conventions
Conventions.
Conversion
Conversion of Strings and Numbers
Corner
Counting
Cut Program
Data
Data Files for the Examples
Data Type Summary
Database
Definition Syntax
Delete
Describes
Details
Distribution
Distribution contents
Distribution,
Do Statement
Do specified action while looping until some condition is satisfied.
Document
Domain
Downward Compatibility and Debugging
Duplicating Output Into Multiple Files
Dupword Program
Dynamic
END {
ENVIRON
Edition
Effective AWK Programming
Egrep Program
Elements
Eliminating duplicate entries from a history file.
Empty
Emulating multi-dimensional arrays in
Environment Variable
Equivalent to specifying
Escape Sequences
Examining Fields
Examples
Executable Scripts
Exit Statement
Explicit Input with
Expression
Expression Patterns
Expressions
Expressions as Patterns
Extensions in the Bell Laboratories
Extract Program
Extracting
Extracting Programs from Texinfo Source Files
FIELDWIDTHS
FILENAME
False
Features
Field
Field Separators
Field Splitting Summary
Field.
Fields
Fields Summary
Files
Files needed for building
Filetrans Function
Finally,
Finding Duplicated Words in a Document
Finding duplicated words in a document.
Fixed-width
For Statement
For maximum portability, do not use the
For numeric conversions, prefix positive values with a space, and negative values with a minus sign.
Format Modifiers
Format-Control Letters
Foundation
Foundation.
Free
Full
Function Calls
Function Caveats
Function Definition Syntax
Function Example
Functions Summary
Functions for Dealing with Time Stamps
Functions for dealing with time stamps.
Functions for files and shell commands.
Functions for getting group information.
Functions for getting user information.
Functions for string manipulation, such as
Functions for using characters as numbers and vice versa.
Functions that work with numbers, including
Functions.
Future Extensions
GNU
GNU GENERAL PUBLIC LICENSE
GNU Regexp Operators
Gawk
Gawk Distribution
Gawk Summary
General
Generating Word Usage Counts
Getline
Getline Intro
Getline Summary
Getline/File
Getline/Pipe
Getline/Variable
Getline/Variable/File
Getline/Variable/Pipe
Getopt Function
Gettimeofday Function
Getting Started
Glossary
Group Functions
Group-ID
Historical Features
History Sorting
How
How Input is Split into Records
How Much Text Matches?
How and why to use BEGIN/END rules.
How fields are split with single characters or simple strings.
How to Use Regular Expressions
How to best name private global variables in library functions.
How to change an element of an array.
How to do case-insensitive matching.
How to examine one element of an array.
How to redirect output to multiple files and pipes.
How to run
How to use numbers as subscripts in
How to write definitions and what they mean.
How to write non-printing characters.
How variables acquire types, and how this affects comparison of numbers and strings with
However,
I/O And BEGIN/END
I/O Functions
I/O Summary
IGNORECASE
Id Program
If Statement
Igawk Program
Immediately exit the innermost enclosing loop.
Implementing
Improvements
In the
Increment Ops
Incrementing the numeric value of a variable.
Index
Info
Input file names and variable assignments.
Input/Output
Installation
Installing
Instead,
Intro
Introduction
Introduction to Arrays
Introduction to the
Invoking Gawk
January
Join Function
Kernighan, Brian
Kinds of Patterns
Known Bugs
LICENSE
Labels Program
Language
Language History
Language Summary
Language.
Languages
Leftmost Longest
Letters
Library
Library Functions
Library Names
License.
Long
Longest
Look
Loop until some condition is satisfied.
MS-DOS
MS-DOS and OS/2 Installation and Compilation
Major Changes between V7 and SVR3.1
Makefile
Making Additions To
Making Each Character a Separate Field
Making each character a separate field.
Managing the Time of Day
Manipulation
Manual History
Merging an Array Into a String
Michael
Minor changes between System V Releases 3.1 and 4.
Miscellaneous Programs
Mktime Function
Mode
Modifiers
More Complex
Multi-dimensional
Multi-dimensional Arrays
Multi-scanning
Multiple Line
Multiple statements may be put on one line by separating each one with a
Multiple-Line Records
Names
Naming Library Function Global Variables
New features from the Bell Laboratories version of
New features from the POSIX standard.
New features that may be implemented one day.
Newlines do not act as whitespace to separate fields when
Next Statement
Next:
Nextfile Function
Nextfile Statement
Node:
Non-Constant Fields
Non-alphabetic
Non-constant Field Numbers
Note
Notes
Noting Data File Boundaries
Numbers.
Numeric
Numeric Array Subscripts
Numeric Built-in Functions
Numeric Functions
OFMT
Obsolete
One-shot
One-shot Throw-away
Only POSIX regexps are supported, the GNU operators are not special (e.g.,
Operator Precedence (How Operators Nest)
Operator Summary
Operators
Operators specific to GNU software.
Operators.
Ops
Options
Ordinal Functions
Other Arguments
Other Command Line Arguments
Other Features
Other Versions
Other freely available
Output Separators
Overview
Overview of Actions
PC Installation
POSIX
POSIXLY_CORRECT
Pairs of patterns specify record ranges.
Passwd Functions
Pattern Elements
Pattern Overview
Pattern Summary
Patterns and Actions
Patterns and Actions, and their component parts.
Philosophy
Pipes
Plain Getline
Portability Notes
Practical
Precedence
Preface
Prev:
Print Examples
Printf Examples
Printf Summary
Printing
Printing Mailing Labels
Printing Non-duplicated Lines of Text
Printing Out User Information
Printing Output
Probable Future Extensions
Problems
Processing
Processing Command Line Options
Programming
Programs
Public
Pulling out programs from Texinfo source files.
Quick Installation
Quick overview of regular expressions.
RLENGTH
Ranges
Re-inventing Wheels for Fun and Profit
Read Terminal
Reading Files
Reading Fixed-width Data
Reading Input Files
Reading files under explicit program control using the
Reading the Group Database
Reading the User Database
Recapitulation of the command line.
Redirecting Output of
Redirections
Reference to Elements
Regexp
Regexp Constants
Regexp Field Splitting
Regexp Operators
Regexp Patterns
Regexp Summary
Regexp Usage
Regexps
Regexps.
Regular Expression Constants
Regular Expression Operators
Regular Expressions
Release
Removing Duplicates from Unsorted Text
Reporting Problems and Bugs
Return Statement
Robbins,
Round Function
Rules
Rules Summary
Running
Running gawk
SUBSEP
Sample Data Files
Sample Programs
Scalar Constants
Scanning
Scanning All Elements of an Array
Scanning an Array
Scripts
Searching for Regular Expressions in Files
See
Separators
Sequences
Setting
Setting variables on the command line and a summary of command line syntax. This is an advanced method of input.
Simple
Simple Sed
Since
Single Character Fields
Skip to the end of the innermost enclosing loop.
Software
Some final points and a summary table.
Some undocumented but supported ``features''.
Sorting
Special File Names in
Special File Summary
Special Files
Special Patterns
Special file names interpreted internally.
Specifying How Fields are Separated
Specifying Record Ranges with Patterns
Specifying initialization and cleanup rules.
Specifying the value a function returns.
Split Program
Splitting
Splitting a Large File Into Pieces
Started
Startup and Cleanup Actions
Statement
Statements
Statements Versus Lines
Statements for Fancier Printing
Statements/Lines
Stop processing the current input record.
Stream
String Concatenation
String Constants Summary
String Functions
String, numeric, and regexp constants.
Subdividing or combining statements into lines.
Subscripts
Suggestions for improvements by volunteers.
Summary
Suppose
Syntax
System
Tee Program
Terminal
Texinfo
Text
The
The GAWK Manual
The Hitchhiker's Guide to the Galaxy
The built-in functions
The conversion of strings to numbers and vice versa.
The empty pattern, which matches every record.
The evolution of the
The field separator and how to change it.
The following command prints all records in
The following program,
The major changes between V7 and System V Release 3.1.
The output separators and how to change them.
The plus sign, used before the width modifier (see below), says to always supply a sign for numeric conversions, even if the data to be formatted is positive. The
The return value of the
The value of
The variable
There
Therefore,
These
Think
This Manual
This string controls conversion of numbers to strings (see
This variable is a
Thus,
Time Functions
Time Functions Summary
Translate Program
Translating Between Characters and Numbers
True if
Truth Values
Turning Dates Into Timestamps
Two Rules
Type
Typing and Comparison
Undocumented
Uninitialized
Uninitialized Subscripts
Uniq Program
Unix
Unix Installation
Unless
Up:
Usage
Use an "alternate form" for certain control letters.  For
Useful One Line Programs
User-ID
User-defined
User-defined Functions
User-modified
Using
Using BEGIN/END
Using Constant Regexps
Using Dynamic Regexps
Using Numbers to Subscript Arrays
Using Regular Expression Constants
Using Regular Expressions to Separate Fields
Using Uninitialized Variables as Subscripts
Using Uninitialized variables as subscripts.
Using Variables
Using no input files (input from terminal instead).
Using regexps as the field separator.
Using this Info file. Includes sample input files that you can use.
Utilities
VMS Compilation
VMS Installation
VMS Installation Details
VMS Running
Variable
Variable Typing and Comparison Expressions
Variables
Variables give names to values for later use.
Variables/Fields
Very Simple
Warn about constructs that are not available in the original Version 7 Unix version of
Wc Program
What Is Awk
What is ``true'' and what is ``false''.
When
When and how to use a regexp constant.
While Statement
Whitespace
Word Sorting
_abandon_
_gr_groupsbyuser
_oldfilename
_opti
_pw_init
_tm_months[
a literal
aardvark
abbreviation
ability
about
above
above,
access
accidentally
action
action.
actual
actually
added
adding
addition
addition,
additional
addresses
advanced
advice
against
alarm
all
allow
allowed
allows
allows access to inherited file descriptors.
alpo-net
alpo-net     555-3412     2400/1200/300     A
already
also have their statements automatically continued on the following line.  In other cases, a line can be continued by ending it with a
alternative
always
ambiguous
and
and so on).  Characters described by octal and hexadecimal escape sequences are treated literally, even if they represent regexp metacharacters.
and/or
anonymous
another
any
anything
anywhere
appear
appended
applies
appropriate
are
are:
argument
argument,
argument.
arguments,
arguments.
arithmetic
arnold@gnu.org
arnold@gnu.org,
around
array.
arrays.
as a Function
assert
assertion
assertions
assign
assigned
assigning
assignment
assignment:
assignments
associated
associative
attempt
automatically
available
available,
available.
avoid
awk
awk '# find smallest divisor of num
awkPrograms
back
backslash
backslashes
backslashes.
based
because
becomes
been
beginfile
beginning
begins
behavior
behavior.
being
between
blank
body
boolean
boolean2
both
braces
brackets
break
building
built-in function is not supported (see
but
call
called
calling
camelot      555-0542     300               C
can
cannot
case
case,
case-insensitive
causes
certain
change
changed
changed.
changes
chapter
character,
character, and continue until the end of the line.  Blank lines may be used to separate statements.  Statements normally end with a newline; however, this is not the case for lines ending in a
character.
characters
characters,
characters.
checking
classes
close
code,
code.
collating
columns
command,
commands
commands.
comment
common
comparisons
compatibility
compatible
compile
compiler
complete
complicated
component
compound
computed
computer
concatenate
concatenated
concatenation.
condition
conditional
conditions
config.h
configuration
configure
consecutive
consequence
consider
considered
consisting
consists
constant
constant,
constant.
constants,
constants.
constructs
contain
containing
contains
contains unparenthesized operators other than
continuation
continuation:
continue
continues
contrast,
contributed
control
controls
convenient
convention
convert
converted
converting
copies
copy
copyright
corresponding
could
count
count_only)
counts
covered
created
curly
current
currently
dark corner <
data.
database,
database.
date
dealing
debugging
decimal
decrement
default
default,
default.
defined
definition
depending
describe
described
describing
description
descriptor
details.
determine
diagnostic
difference
differences between
different
difficult
digits
directly
directory
directory.
discussed
distribute
distributed
distribution.
documentation
documentation.
documented
does
does not exist, it is created. If it does exist, its contents are deleted the first time the
does not exist, then it is created.
doesn't
doing
don't
done
double
double-quote
duplicate
duplicated
each
easier
easy
echo
echo @include "$f" >> /tmp/ig.s.$$ ;;
effect
effective
effectively
egrep
either
electronic
element
element,
elements and the elements of an array created by
elements.
else
enclosed
enclosing
end
endfile
ending
entire
entry
environment
environment.
equal
equivalent
equivalent.
error
error.
errors, common
escape
escape sequences are not recognized (see
essentially
evaluated
even
every
exactly
example
example,
example.
example:
except
executable
execute
executed
executed,
executed.
executes
executing
execution
exit
exit 1
exits.
explicit
expr
expr-list
expression,
expression.
expression:
expressions,
expressions.
expressions:
extension. In other
extension; it is not available in compatibility mode (see
extensions
extra
extract
extract.awk
facilities
familiar
fancier
feature
features.
features:
fflush
field,
fields,
fields.
fieldsep
file
file,
file.
files,
files.
final
find
first
first,
flag sets the maximum number of fields, and the
floating
fmt, expr-list
follow
followed
following
following:
follows
follows:
foo
for
for (i = 1; i < Optind; i++)
for (i = 1; i <= NF; i++)
for (i in copy)
force
form
format
format-control
formats, it specifies the maximum number of significant digits.  For the
formats, this specifies the number of digits you want printed to the right of the decimal point.  For the
formatted
four
freely
frequencies
from
function
function (see
function is called to ensure that if two identical
function,
function.
function:
functions
functions,
further
future
gawkon
generally
generate
generated
generates
gensub
get
getline
getopt
getopt(argc,
getting
give
given
gives
global
good
group
gsub
handle
happens
has
has been specified on the command line (see
have
haven't
having
help
here
hexadecimal
historical
historical features
history
hours
i++)
identical
identifier_regexp
if (pw != "") {
if you want your program to be portable to other
igawk
ignore
ignored.
illustrates
immediately
immediately.
implementation
implementation,
implementations
implementations generate a run-time error if you use the
implementations limit the number of pipelines an
implementations, or if
implementations.
implementations.  (It happens that
implemented
important
include
included
includes
including
increment
incremented
increments
indexed
indicate
indicates
indices
individual
information
information.
information:
initial
initialization
initialize
initialized
input
input,
input-file
input.
input:
inside
install
installing
instead
instead of
instructions
integer
intended
interactive
interested
interesting
internal
interpret
interpretation
interpreted
interval
into
introduced
introduced these features:
invalid
inventory-shipped
is ambiguous because the concatenation operator is not parenthesized, and you should write it as
is ambiguous if
is computed next and its value becomes the value of the whole expression.
is in compatibility mode (see
is provided on the command line (see
is stored.  Since it is used to help out an
is used so that the function fits nicely on the
issues
it.
items
its
itself
just
keep
keyword
know
known
language, POSIX version <
languages,
languages.
large
last
later
leading
least
left-hand
length
less
letter
level,
library function (see
library routine, we have chosen to put it in
like
line
line continuation
line,
line.
lines
lines,
lines.
list
listed
lists
literal
local
locale's
longer
looks
loop
loop.
lower-case
lvalue
made
mailing
main
make
makes
making
manual
many
match
match.
matched
matched.
matches
matches the empty string
matching
maximum
may
meaning
means
memory
message
message.
method
might
missing
mistakes, common
mktime
modifications
modified
month
more
most
much
multi-character
multiple
must
name
name.
named
names.
necessarily
necessary
necessary.
need
needed
never
new
newline
newline,
newline.
newlines
next
next file
nextfile
non-option
non-repeated
non-zero
normal
normally
normally,
not
not in POSIX
notation,
nothing
notice
now)
null
number
number,
number.
numbers
numbers,
numbers:
numeric,
occurrence
occurrences
often
omitted,
on the Atari ST
on the command line does not set the value of
once
once,
one
one,
one.
only
open
operand
operands
operating
operations
operations.
operator
operator <
operator is not specified by POSIX.)
operator,
operator.
operator:
operators,
operators:
optarg
optind
option
option.
option:
optional
options,
options.
options:
or 
order
ordinary
original
other
otherwise
otherwise,
out
output
output,
output-file
output.
output:
outputfile
outside
over
parameter-list
parameters
parentheses
parentheses.
parenthesized,
part
particular
particular,
particularly
parts
passed
password
path
pattern
pattern (see
pattern,
pattern.
patterns
patterns.
people
perform
performed.
pieces
pipe
pipeline
place
please
point
portability
portability issues
portable
position
positive
possible
precedence,
preceding
precision
presented
previous
previously
print
print ""
print "A strange truth value"
print $0
print e > "/dev/stderr"
printed
printed,
printed.
printf
printf "%s\t%d\n", word, freq[word]
printf "Smallest divisor of %d is %d\n", num, div
printf("%4d %s\n", count, last) > outputfile
printf("(%s)", a[1])
prints
private
probably
problem
problem.
process
process.
processed
processed,
processed.
processing,
processing.
produce
produces
program
program may have open to just one!  In
program's
program,
program-file
program-text
program.
program.  If the end of the input data is reached, the
program:
programmer
programs illustrated in this Info file.
programs,
programs.
programs; includes command line syntax.
project
provide
provided
provides
purpose
quotes
random
range
rather
read
read,
read.
reading
reads
real
really
reason
recognized
recommend
record
record,
record.
records
records.
redirect
redirected
redirection
redistribute
reference
regex
regexp operator:
regexps are matched. The GNU operators are not special, interval expressions are not available, and neither are the POSIX character classes (
regular
related
relational operator; otherwise it could be confused with a redirection (see
remaining
remove
repeated
replace
replaced
replacement
report
reports
represent
representation
representation.
represented
represents
require
required
requires
rest
result
result.
resulting
return
return ""
returned
returns
right
right-hand
routines
rule
rule sets a private variable to the directory where
rules.
run
run-time
runs
sabafoo
same
sample
search
searches
second
seconds
section
sections
self-contained
separate
separated
separating
separator
separator,
separator.
sequence
series
set
sets
sets the variable
several
shell
shell,
short
should
should only be used inside the body of a loop (d.c.).
shows
side
significant
similar
simply
single
skip
software,
software.
some
something
sometimes
source
source-file
space
space.
spaces
spaces,
special
specific
specification
specifications
specified
specified,
specifier
specifies
specify
specifying
split
split(pw, a, ":")
splitting.
sprintf
square
standard
standard,
standard.
standards
start
starting
starts
statement (see
statement causes the end of the input to be reached, then the code in any
statement has no meaning when used outside the body of a loop.  However, although it was never documented, historical implementations of
statement outside of a loop as if it were a
statement removes an element from an array.
statement,
statement.
statement. It loops through the indices of an array's existing elements.
statement:
statements,
statements.
status
still
stored
strftime
string
string comparison (true)
string,
string.
strings
strings,
strings.
style
sub
subexpressions
subscript
subscripts:
subsequent
substitutions
substr
substr(argv[Optind],
substring
successive
such
supplied
supplied,
supplied.
supplies
supply
support
supported
sure
symbol
system call.  (Multiple groups may not be supported on all systems.)
system,
system.
systems
systems,
systems.
table
taken
takes
target
temporary
terminated
terms
test
tested
text,
than
that
the
their
them
them.
then
therefore
they
thing
things
third
this
this,
this:
those
though
three
through
throw-away
time
time.
times
timestamp
timestamp.
to a string describing the error that occurred.
to be recomputed, with the fields being separated by the value of
together
total
track
traditional
trailing
translate
translation
transliteration
treated
true
true,
two
two-dimensional
typically
typing
unchanged.
undefined
under
under VMS.
understand
unparenthesized
until
upon
upper-case
use
used
used,
used.
useful
user
user's
users
uses
using
usually
utility.
utility:
valid
value
value,
value.
values
values.
var
variable,
variable.
variables,
variables.
variables:
various
version
version.
version:
versions
very
vmsbuild.com
want
warning
was
way
week
well
well.
were
what
whatever
where
whether
which
while
whole
whose
width
will
wish
with
within
without
word
words
words,
work
work.
works
would
write
writing
written
year
you
your
zero
zero,
zero.
{ printf "%-10s %s\n", $1, $2 }' BBS-list
} else
