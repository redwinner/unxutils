"<="
"Conditions
"parse error"
#define
#define YYPARSE_PARAM parm
#define YYSTYPE double
#include
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#line
$1);
$2;
$3;
%expect
%left
%left '
%left '*' '/'
%left '+' '-'
%left '-' '+'
%left NEG     /* negation--unary minus */
%no_lines
%nonassoc
%prec
%pure_parser
%right
%right '^'    /* exponentiation        */
%s\n",
%start
%token
%token NUM
%token_table
%type
%union
%union {
'('
')'
'*'
'+'
','
'-'
';'
'\n'
'{' declarations statements '}'
'{' statements '}'
'}'
("%s\n",
("\t%.10g\n",
("empty
($1,
();
(Bison
(Reentrant)
(This
(algebraic)
(and
(char
(file,
(grouping
(in
(introduction):
(or
(see
(sym_name)
(symbuf,
(symrec
(terminal
(the
(token
(token_buffer)
(which
(yytname[i]
) is returned to
*ptr;
*sym_name;
*tptr;
, Prev:
, Up:
, and
, and its address is passed to
, and so on.
, the names become
, with a
--file-prefix=
--name-prefix=
--output-file=
.  See
.  When
.)  See
...
.tab.c
/* Grammar follows */
/* empty */
02111-1307,
1 + 2
2.3000000000
3.141592653
<ctype.h>
<math.h>
<operator>
<stdio.h>
<val>
A Pure (Reentrant) Parser
ASCII
ASSUME
About Makertf
About this help file
Action Features
Action Types
Actions
Actions in Mid-Rule
Acts like a variable that contains the semantic value for the
Additional C code
Algorithm
All kinds of Bison declarations are described here.
All the options described in detail, in alphabetical order by short options.
Alphabetical list of long options.
Alternatively,
An action is the semantic definition of a grammar rule.
An example showing why precedence is needed.
Analyzer
BEING
BOGUS
Bison
Bison Declaration Summary
Bison Declarations
Bison Grammar Files
Bison Options
Bison Output: the Parser File
Bison Parser
Bison Symbols
Bison and C declarations for rpcalc.
Bison command syntax on VMS.
Bison declaration
Bison declaration (see
Bison declaration to
Bison declarations for multi-function calculator.
Bison parser
Bison's
Bison,
Bison.
C Code
C Declarations
C-Language
Calc
Calculator
Calculator with memory and trig functions. It uses multiple data-types for semantic values.
Calculator:
Calling
Calling Convention
Calling Convention for
Calling Conventions for Pure Parsers
Code
Collection
Compile
Compiling the Parser File
Concepts
Conditions for Using Bison
Conflicts
Conflicts: when either shifting or reduction is valid.
Context
Context Dependency
Context-Dependent Precedence
Contextual Precedence
Continuing after syntax errors.
Convention
Conventions
Copying
Cross
Data Types of Semantic Values
Data Types of Values in Actions
Debugging
Debugging Your Parser
Decl
Decl Summary
Declarations for
Declare
Declare a terminal symbol (token type name) that is
Declaring terminal symbols.
Declaring terminals with precedence and associativity.
Declaring the choice of type for a nonterminal symbol.
Declaring the set of all semantic value types.
Decls
Defining Language Semantics
Dependencies
Dependency
EVENT
EXPRESSED
Each rule can have an action containing C code.
Each token or syntactic grouping can have a semantic value (the value of an integer, the name of an identifier, etc.).
Error Recovery
Error Reporting
Error-recovery
Examples
Exercises
Expect Decl
Explanation of
Expr
External variable in which
FNCT
Features
Files
For example:
Foundation
Foundation,
Foundation.
Free
From Formal Rules to Bison Input
GENERAL,
GNU
GNU GENERAL PUBLIC LICENSE
General
Glossary
Grammar
Grammar File
Grammar Layout
Grammar Outline
Grammar Rules
Grammar Rules for
Grammar Rules for rpcalc, with explanation.
Grammar in Bison
Grammar rules for the calculator.
Handling Context Dependencies
Here
Here we assume that
How
How Precedence
How Precedence Works
How the calling convention differs in a pure parser (see
How these features are used in the previous example.
How they work.
How to call
How to specify precedence in Bison grammars.
How to write grammar rules.
How we represent grammars for Bison's sake.
However,
IF expr THEN stmt
INT;
INTEGER
Ideas for improving the multi-function calculator.
Ignore
Index
Infix (algebraic) notation calculator. Operator precedence is introduced.
Infix Calc
Infix Notation Calculator:
Initially
Interface
Introduction
Invocation
Invoking Bison
Invoking Bison under VMS
Key
LALR(1)
LALR(1).
LR(1)
Language and Grammar
Languages and Context-Free Grammars
Languages and context-free grammars, as mathematical ideas.
Layout
Lexer
Lexical Tie-ins
Lexical Tie-ins and Error Recovery
Lexical tie-ins have implications for how error recovery rules must be written.
Library
License
License,
License.
Line
Listing
Look-Ahead
Look-Ahead Tokens
Macro for specifying the
Main
Makertf
Many
Mfcalc Decl
Mfcalc Rules
Mfcalc Symtab
Mid-Rule
Mid-Rule Actions
More Than One Value Type
Most actions go at the end of a rule. This says when, why and how to use the exceptional action in the middle of a rule.
Multi-Function Calculator:
Multi-function Calc
Multiple Parsers
Multiple Parsers in the Same Program
Multiple Types
Mysterious Reduce/Reduce Conflicts
Mystery Conflicts
NECESSARY
NUM
Next:
Node:
Nonterminal Symbols
Note
OUT
Once
Operator Precedence
Operator precedence works by resolving conflicts.
Option Cross Key
Options
Outline
Outline of a Bison Grammar
Overall
Overall layout of the grammar file.
Overall structure of a Bison grammar file.
Overflow
Overflow,
POSSIBILITY
PURPOSE.
Parser C-Language Interface
Parser Function
Parser States
Parser looks one token ahead when deciding what to do.
Place
Positions
Precedence Decl
Precedence Examples
Prev:
Program's
Program),
Public
Pure Calling
Pure Decl
Putting more than one Bison parser in one program.
RPN Calc
Recovery
Recursion
Recursive Rules
Reduce/Reduce
Reduce/Reduce Conflicts
Reduce/reduce conflicts that look unjustified.
Reference
Requesting a reentrant parser.
Return immediately from
Reverse Polish Notation Calculator
Reverse polish notation calculator; a first example with no operator precedence.
Routine
Rpcalc Compile
Rpcalc Decls
Rpcalc Error
Rpcalc Expr
Rpcalc Gen
Rpcalc Input
Rpcalc Lexer
Rpcalc Line
Rpcalc Main
Rpcalc Rules
Run the C compiler on the output code.
Running Bison on the grammar file.
Running Bison to Make the Parser
Section
See
Semantic Actions
Semantic Info in Token Types
Semantic Tokens
Semantic Values
Semantic Values of Tokens
Semantic values and actions.
Semantics
Shift/Reduce Conflicts
Simple Error Recovery
Software
Special Features for Use in Actions
Special features for use in actions.
Specifying Operator Precedence
Specifying data types for actions to operate on.
Specifying one data type for all semantic values.
Specifying several alternative data types.
Specifying the start symbol.
Stack Overflow
Stack Overflow, and How to Avoid It
Stages
Stages in Using Bison
Stages in writing and running Bison grammars.
Start Decl
Start-Symbol
States
Summary
Suppressing Conflict Warnings
Suppressing warnings about shift/reduce conflicts.
Symbol Table
Symbol table management subroutines.
Symbols
Symbols, Terminal and Nonterminal
Symtab
Syntax and usage of the Bison declarations section.
Syntax and usage of the C declarations section.
Syntax and usage of the additional C code section.
Syntax and usage of the grammar rules section.
Syntax of Grammar Rules
TYPENAME
Table of Symbols
Table of all Bison declarations.
Terminal and nonterminal symbols.
Textual Positions of Tokens
The
The Additional C Code Section
The Bison Declarations Section
The Bison Parser Algorithm
The C Declarations Section
The Collection of Value Types
The Concepts of Bison
The Controlling Function
The Error Reporting Function
The Error Reporting Routine
The Grammar Rules Section
The Lexical Analyzer Function
The Overall Layout of a Bison Grammar
The Start-Symbol
The controlling function.
The error reporting function.
The function
The lexical analyzer.
The number of
The parser function
The parser is a finite-state-machine with stack.
The value returned by
Therefore,
Thus,
Tie-in Recovery
Tie-ins
Token Decl
Token Positions
Token Type Names
Token Values
Token parsing can depend on the semantic context.
Token parsing can depend on the syntactic context.
Tokens
Top
Type
Type Decl
Types
UMINUS
Unary
Union Decl
Up:
Using
Using Precedence
VAR
VMS Invocation
Value Type
Values
WARNING
What are Bison's input and output, how is the output used?
What happens when stack gets full.  How to avoid it.
When Precedence is Needed
When an operator's precedence depends on context.
When two rules are applicable in the same situation.
Why Precedence
Writing recursive rules.
YYACCEPT
YYDEBUG:
YYERROR;
YYERROR_VERBOSE
YYINITDEPTH
YYLEX_PARAM
YYLTYPE
YYMAXDEPTH
YYNRULES
YYNSTATES
YYNTOKENS
YYPARSE_PARAM
YYPRINT
YYRECOVERING
YYSTYPE
Yacc-compatible
You must supply a function
`declaration'
`expression'
`return',
`statement',
abort
about
above
accept
acceptable
acceptable.
access
accessible
accompanies
according
action
action,
action.
actions,
actions.
actions:
active
actual
actually
add
added
adding
addition
addition,
additional
address
addressed
after
afterward.
all
allocated
allocation
allow
allowed
allowed.
allows
along
alphabetical
alphabetized
already
also
alter
alternate
alternative
alternative.
always
ambiguity:
among
analyzer,
analyzer.
analyzer:
analyzer¢
analyzing
and
and what it returns.
and/or
another
any
anything
appear
appears
applicable
applied
applies
apply
appropriate
are
argument
argument,
argument.
arguments
arithmetic
arrange
array
assign
assignment
associate
associated
association
associativity
associativity,
associativity.
attach
attempt
author
automatically
available
avoid
back
based
basic
because
become
becomes
been
before
begin
beginning
beginning.
between
binary
bison:
both
built
but
but specifies alternative
calculator,
calculator.
calculator;
calculators
call
called
calls
can
cannot
case
case,
case.
cause
causes
certain
chain
change
changed
changes.
chapters
char *s;
char *sym_name;
character
character.
character;
characters
characters,
characters.
charge
choice
choose
choosing
class
clear
close-delimiter
code,
code.
codes
combined
comma-separated
command
commands
commit
communication
compilation
compiler
compiler,
complete
component
component,
components
components.
compound
compound: { prepare_for_local_variables (); }
compute
computes
conditions
conflict
conflict:
conflicts
conflicts,
conflicts.
consequence
consequence,
consider
considered
consists
constant
constants
construct
construct.
constructed
constructs
constructs,
constructs.
contain
containing
contains
contents
context-
context-free
context.
contexts
contexts,
continue
continue.
contrast,
control
controlled
controlling
convention,
conventions.
converted
converts
copied
copies
copy
copy,
copying,
copyright
copyrighted
correct
corresponding
could
countries
course,
covered
create
current
customarily
data
data-types
debuggers
decide
deciding
decision
declaration
declaration (see
declaration makes
declaration,
declaration.  See
declarations
declarations,
declarations.
declarations:
declarator
declare
declared
declared.
declares
declaring
declaring:
default
default.
define
defined
defines
defining
definition
definitions
depend
depending
depends
derivative
derived
describe
described
describes
designated
designed
desirable
desk-top
details
details.
detected:
determine
determined
difference
different
directly
discards
distinct
distinction
distinguish
distinguished
distribute
distributed
distributing
distribution
documents
does
doesn't
don't
done
double
double val;
double-quote
during
each
effect
effectively
either
elements
else
else-clause
embedded
empty
encountered
end
end-of-file
end-of-input
end-of-input).
end-of-input.
end-of-rule
enforces
entire
entirely
entitled
entry
equal
equally
equivalent
equivalent:
erroneous
error
error reporting
error"
error,
error-reporting
error.
error:
errors
errors.
even
eventually
every
everyone
everything
example
example,
example.
example:
except
exceptional
executable
executable.
executed
executed.
executes
exercise
exit.
exp
exp:     
exp:      NUM
exp:    ...
explain
explained
explained.
explanation.
explicit
explicitly
exponentiation
express
expression
expression,
expression.
expressions
expressions.
expressions:
expseq1
expseq1:  exp
extra
facilities,
feature
file
file,
file-prefix
file.
file:
file_name
filename yields
files,
files.
find
finite-state
finite-state-machine
first
first,
floating
follow
followed
following
follows
follows,
follows:
foo.tab.c
foo.y
for
form
form:
formal
found,
four
freedom
from
function
function,
function.
function:
functions
functions,
functions.
further
generate
generated
generates
generating
generator
get
getchar
gets
getsym
give
given
global
going
grammar
grammar's
grammar,
grammar.
grammar:
grammars
grammars,
grammars.
grammatical
grammatically
granted
group
grouping
grouping.
groupings
groupings,
groupings.
hack/foo.
hand
handle
handles
handling
happens
happens,
hard
has
have
header
help
here,
here.
hex
hex (
hexflag
higher
higher,
holder
how
hypothetical
identical
identifier
identifier,
identifiers
if (c == '.' || isdigit (c))
if (c == EOF)
if x then if y then win (); else lose;
if-statement
if-statement,
if_stmt:
immediate
immediately
immediately.
implemented
implications
important
improving
in the union specified by the
inadequate
include
included
includes
including
independent
indicate
indicates
indicating
indirectly
infix
information
information,
information.
init
init_table
initialization
innermost
input
input,
input.
input:    /* empty */
inputs
inserted
inside
installation
instance
instances
instead
instead of
instead.
int
int nastiness;
int randomness;
integer,
integer.
integers
intended
interactive
interchange;
interface:
internal
interpreted
interpreter,
into
introduced.
introduces
invalid
invoking Bison
involves
is the name
it,
it.
its
itself
itself.
joined
just
keep
keyword
keyword,
kind
kinds
know
known
language
language,
language.
languages
languages,
languages.
larger
last
later
latter
leading
leads
least
left
left-associative
leftmost
legitimate
length
lexical
lexical analyzer
licensed
licensee
licenses
like
limitation
limited
line:     '\n'
list
literal
literal,
literal:
literals
literals,
little
local
location
long
look
looks
lose;
lower
machine
machine-readable
machine.
macro
macros
made
make
makes
making
management
manual
manual.
match
mathematical
matter
may
maybeasm
maybeword
meaning
meaningful
meanings
means
medium
members
members,
members.
memory
message
messages
messages,
mfcalc
middle
might
minus
modification
modifications
modified
modifier
modify
modifying
more
most
much
multi-character
multi-function
multiple
must
must return the semantic value of the token it has read.
must return the text position (line number, etc.) of the token, if the actions want that.
name
name of author
name)
name,
name.
name_list
named
names
names.
nastiness;
necessarily
need
needed
needed.
needs
negation--unary
nested
never
new
newline
newline.
next
non-free
nonreentrant
nonterminal
nonterminal,
nonterminals
nonterminals,
nonterminals.
nonterminals:
nonzero
normally
not
not,
notation
notation:
nothing
nothing-at-all
notice
noticeably
notices
notype_declarator
now
number
number,
number.
numbers
numbers.
numeric
object
occurs
offer
offering
often
on.
one
one.
only
onto
open-brace
operands
operate
operation.
operator
operator precedence
operator's
operator.
operators
operators.
option
option when you run Bison, so that it will write these macro definitions into a separate header file
option.
optionally
options.
or 
order
ordinarily
ordinary
original
other
otherwise
outfile
output
overflow.
overflow:
param_spec
param_spec:
parameter
parentheses
parm
parse
parsed
parsed,
parsed.
parser
parser,
parser.
parser:
parser_control
parsers
parsing
parsing.
part
particular
particular,
parties
parts
parts.
pass
passed
patent
people
performs
permission
permit
permitted
permitted.
piece
placed
plus
point
point,
pointer
pointers
polish
portion
position
position.
possible
possible.
postfix
practical
precedence
precedence,
precedence.
precedence:
precedes
precise
preferred
prefix
prepare_for_local_variables
preprocessor
pretend
prevent
previous
primarily
primary
print
printf
printf ("%s\n", s);
probably
problem
problems
process
processing
produce
produced
produces
program
program,
program.
program;
programmer
programming
programs
programs,
programs.
programs:
proper
proprietary
proprietary.
protection
provide
provided
provides
published
punctuation
pure
purpose
purposes
purposes.
put
putsym
randomness;
rather
read
read,
read.
reading
reads
real
reason
receive
received
receives
recipients
recognize
recognized
recognized.
recognizes
recover
recovering
recovery.
recursion,
recursion:
recursive
redeclare
redirect
redirects
redistribute
reduce
reduced
reducing
reduction
reductions
reentrant
reentrant.
refer
referred
refers
refers to the
regardless
relative
removing
repeated
replaced
report
reporting
represent
represented
represents
request
required
requires
reserved
resolution
resolved
resolving
rest
restrictions
result
resulting
results.
resume
return
return 0;
return INT;      /* Return the type of the token. */
return NUM;
return c;
return ptr;
return_spec
return_spec:
returned
returned.
returning
returns
returns.
reverse
right
right.
rights
rpcalc
rpcalc.tab.c
rpcalc.y
rule
rule's
rule,
rule.
rules
rules,
rules.
rules:
run
run.
running
same
sample
satisfy
says
second
section is copied
section.
section:
sections
sections.
sections:
semantic
semantic:
semicolon
semicolon,
sense
sentence
separate
sequence
sequence.
sequence: /* empty */
sequence\n");
sequences
serves
set
several
share
shift
shift/reduce
shifted
shifted,
shifted.
shifted;
shifting
short
should
show
showing
shown
signal
similar
simple
simple:
simply
since
single
single-character
situation
situation.
skip
skips
smaller
software,
software.
solution
some
something
sometimes
source
space
special
specific
specification
specified
specifies
specify
specifying
spurious
stack
stack,
stack.
stand
standard
stands
start
starting
starts
state
statement
statement,
statement.  See
statements
statements.
static
stdin);
steps:
still
stmnt
stmnts
stmt
store
stored
strategies
strategy
stream
string
struct parser_control
structure
subdivided
subexpressions
subprogram
subroutine
subroutine:
subroutines.
subsequent
substitute
such
sufficient
suitable
summary:
supply
supports
suppose
sure
sym_type;
symbol
symbol,
symbol.
symbol:
symbolic
symbols,
symbols.
symrec
symrec *
symrec *ptr;
symrec *tptr;
symrec'.
syntactic
syntactically
syntax
syntax.
syntax:
system
table
table.
take
taken
technique
term
terminal
terminal-symbol
terminals
terminate
terms
text
textual
than
that
that.
the
their
them
them.
then
there
there.
therefore
these
they
thing
things:
third
this
this,
this.
this:
those
three
through
thus
tie-in
time
time,
time.
together
token
token type
token's
token(s).
token,
token-type
token.
token:
tokens,
tokens.
tokens:
trace
track
translations
trouble.
true
two
type,
type.
type:
typealt
typedef
types,
types.
typical
typically
under
understand
ungetc (c, stdin);
uninitialized
union
unjustified.
unless
unrecoverable
until
upper
usage
use
use.
used
used.
useful
uses
usual
usually
utility
utterance
val;
valid
valid,
valid.
value
value type
value)
value,
value.
value;
values,
values.
variable
variable.
variables
variables,
various
verbatim
verbose
version
versions
very
void *
want
warnings
warranty
warranty;
was
way
way.
ways
well
were
what
whatever
when
whenever
where
whether
which
which reads tokens.
which you can include in the other source files that need it.  See
while
whitespace
who
whole
whose
will
wish
with
within
without
word
words
words,
work
works
would
write
writing
written
written.
yields
you
your
yychar
yyclearin
yyclearin;
yydebug
yyerro
yyerrok
yyerrok;
yyerror
yyerror (s)  /* Called by yyparse on error */
yylex
yylex ()
yylloc
yylval
yynerrs
yyparse
yyparse ();
yytname
yytname[i][strlen
zero
{ $$ = $1 + $3; }
{ printf ("added word %s\n", $2); }
{ printf ("empty sequence\n"); }
|    name ':' type
|    name_list ':' type
| '(' error ')'
| '(' expr ')'
| '{' statements '}'
| IF expr THEN stmt ELSE stmt
| error '\n' { yyerrok;                  }
| exp '
| exp '+' exp
| exp '\n'   { printf ("\t%.10g\n", $1); }
| exp '\n'  { printf ("\t%.10g\n", $1); }
| exp exp '+'     { $$ = $1 + $2;    }
| exp exp '-'     { $$ = $1 - $2;    }
| expr '
| expseq1 ',' exp
| input line
| redirects redirect
| sequence redirects
| sequence word
| sequence words
| stmnts e
| words word
¢Bison
¢left
¢lexical
¢literal
¢nonterminal
¢terminal
¢token
