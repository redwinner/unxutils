/* lid.c -- primary query interface for mkid database
   Copyright (C) 1986, 1995, 1996, 1999 Free Software Foundation, Inc.
   Written by Greg McGary <gkm@gnu.ai.mit.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#include <config.h>
#include <stdio.h>
#include <ctype.h>
#include "xstdlib.h"
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#ifndef WIN32
#include <sys/wait.h>
#endif
#include <assert.h>
#include <getopt.h>
#include <regex.h>
#include "xstring.h"
#include "xunistd.h"
#include "xnls.h"
#include "xmalloc.h"
#include "idfile.h"
#include "xstring.h"
#include "error.h"
#include "pathmax.h"
#include "xalloca.h"
#if HAVE_LIMITS_H
# include <limits.h>
#endif

typedef void (*report_func_t) __P((char const *name, struct file_link **flinkv));
typedef int (*query_func_t) __P((char const *arg, report_func_t));

enum delimiter_style
{
  ds_bogus,
  ds_contextual,
  ds_word,
  ds_substring
};

enum pattern_style
{
  ps_bogus,
  ps_contextual,
  ps_literal,
  ps_regexp
};

enum key_style
{
  ks_bogus,
  ks_none,
  ks_token,
  ks_pattern
};

enum result_style
{
  rs_bogus,
  rs_none,
  rs_filenames,
  rs_grep,
  rs_edit
};

enum radix
{
  radix_oct = 1,
  radix_dec = 2,
  radix_hex = 4,
  radix_all = radix_dec | radix_oct | radix_hex
};

void usage __P((void));
static void help_me __P((void));
void lower_caseify __P((char *str));
enum key_style parse_key_style __P((char const *arg));
enum result_style parse_result_style __P((char const *arg));
query_func_t get_query_func __P((char *pattern));
report_func_t get_report_func __P((void));
void report_filenames __P((char const *name, struct file_link **flinkv));
void report_grep __P((char const *name, struct file_link **flinkv));
void report_edit __P((char const *name, struct file_link **flinkv));
void report_nothing __P((char const *name, struct file_link **flinkv));
int vector_cardinality __P((void *vector));
int search_flinkv __P((struct file_link **flinkv));
int query_literal_word __P((char const *pattern, report_func_t report_func));
int query_literal_prefix __P((char const *pattern, report_func_t report_func));
int query_regexp __P((char const *pattern_0, report_func_t report_func));
char const *maybe_add_word_delimiters __P((char const *pattern_0));
int query_number __P((char const *pattern, report_func_t report_func));
int query_ambiguous_prefix __P((unsigned int, report_func_t report_func));
int query_literal_substring __P((char const *pattern, report_func_t report_func));
void parse_frequency_arg __P((char const *arg));
int desired_frequency __P((char const *tok));
char *strcasestr __P((char const *s1, char const *s2));
char const *file_regexp __P((char const *name_0, char const *left_delimit, char const *right_delimit));
off_t query_binary_search __P((char const *token));
int is_regexp __P((char *name));
int has_left_delimiter __P((char const *pattern));
int has_right_delimiter __P((char const *pattern));
int file_name_wildcard __P((char const *re, char const *fn));
int word_match __P((char const *name_0, char const *line));
int get_radix __P((char const *str));
int is_number __P((char const *str));
int stoi __P((char const *str));
int otoi __P((char const *str));
int dtoi __P((char const *str));
int xtoi __P((char const *str));
unsigned char *tree8_to_bits __P((unsigned char *bits_vec, unsigned char const *hits_tree8));
void tree8_to_bits_1 __P((unsigned char **bits_vec, unsigned char const **hits_tree8, int level));
struct file_link **tree8_to_flinkv __P((unsigned char const *hits_tree8));
struct file_link **bits_to_flinkv __P((unsigned char const *bits_vec));

#if HAVE_TERMIOS_H || HAVE_TERMIO_H || HAVE_SGTTY_H
void savetty __P((void));
void restoretty __P((void));
void linetty __P((void));
void chartty __P((void));
#endif

#define	TOLOWER(c)	(isupper (c) ? tolower (c) : (c))
#define IS_ALNUM(c)	(isalnum (c) || (c) == '_')

/* Sorry about all the globals, but it's really cleaner this way. */

char anchor_dir[BUFSIZ];
int tree8_levels;
unsigned int bits_vec_size;
struct idhead idh;
char *hits_buf_1;
char *hits_buf_2;
unsigned char *bits_vec;

/* The name this program was run with. */

char const *program_name;

/* If nonzero, display usage information and exit.  */

static int show_help;

/* If nonzero, print the version on standard output then exit.  */

static int show_version;

/* Which radixes do we want? */

int radix_flag = radix_all;

/* If nonzero, ignore differences in alphabetic case while matching.  */

int ignore_case_flag = 0;

/* How will patterns will be delimited? */

enum delimiter_style delimiter_style = ds_contextual;

/* How will patterns be interpreted? */

enum pattern_style pattern_style = ps_contextual;

/* How will keys be presented? */

enum key_style key_style = ks_token;

/* How will query results be presented? */

enum result_style result_style = rs_filenames;

/* How shall we separate file names when result_style == rs_filenames?  */

enum separator_style separator_style = ss_contextual;

/* If non-zero, list identifiers that are are non-unique within this
   number of leading characters.  */

unsigned int ambiguous_prefix_length = 0;

/* The style of report.  */

report_func_t report_function;

/* The style of query.  */

query_func_t query_function;

/* Lower and upper bounds on occurrence frequency.  */

unsigned int frequency_low = 1;
unsigned int frequency_high = USHRT_MAX;

struct file_link *cw_dlink;
struct file_link **members_0;

static struct option const long_options[] =
{
  { "file", required_argument, 0, 'f' },
  { "frequency", required_argument, 0, 'F' },
  { "ambiguous", required_argument, 0, 'a' },
  { "key", required_argument, 0, 'k' },
  { "result", required_argument, 0, 'R' },
  { "separator", required_argument, 0, 'S' },
  { "ignore-case", no_argument, 0, 'i' },
  { "literal", no_argument, 0, 'l' },
  { "regexp", no_argument, 0, 'r' },
  { "word", no_argument, 0, 'w' },
  { "substring", no_argument, 0, 's' },
  { "hex", no_argument, 0, 'x' },
  { "decimal", no_argument, 0, 'd' },
  { "octal", no_argument, 0, 'o' },
  { "help", no_argument, &show_help, 1 },
  { "version", no_argument, &show_version, 1 },
  { 0 }
};

void
usage (void)
{
  fprintf (stderr, _("Try `%s --help' for more information.\n"),
	   program_name);
  exit (1);
}

static void
help_me (void)
{
  printf (_("\
Usage: %s [OPTION]... PATTERN...\n\
"), program_name);

  printf (_("\
Query ID database and report results.\n\
By default, output consists of multiple lines, each line containing the\n\
matched identifier followed by the list of file names in which it occurs.\n\
\n\
  -f, --file=FILE       file name of ID database\n\
\n\
  -i, --ignore-case     match PATTERN case insensitively\n\
  -l, --literal         match PATTERN as a literal string\n\
  -r, --regexp          match PATTERN as a regular expression\n\
  -w, --word            match PATTERN as a delimited word\n\
  -s, --substring       match PATTERN as a substring\n\
            Note: If PATTERN contains extended regular expression meta-\n\
            characters, it is interpreted as a regular expression substring.\n\
            Otherwise, PATTERN is interpreted as a literal word.\n\
\n\
  -k, --key=STYLE       STYLE is one of `token', `pattern' or `none'\n\
  -R, --result=STYLE    STYLE is one of `filenames', `grep', `edit' or `none'\n\
  -S, --separator=STYLE STYLE is one of `braces', `space' or `newline' and\n\
                        only applies to file names when `--result=filenames'\n\
            The above STYLE options control how query results are presented.\n\
            Defaults are --key=token --result=filenames --separator=%s\n\
\n\
  -F, --frequency=FREQ  find tokens that occur FREQ times, where FREQ\n\
                        is a range expressed as `N..M'.  If N is omitted, it\n\
                        defaults to 1, if M is omitted it defaults to MAX_USHRT\n\
  -a, --ambiguous=LEN   find tokens whose names are ambiguous for LEN chars\n\
\n\
  -x, --hex             only find numbers expressed as hexadecimal\n\
  -d, --decimal         only find numbers expressed as decimal\n\
  -o, --octal           only find numbers expressed as octal\n\
            By default, searches match numbers of any radix.\n\
\n\
      --help            display this help and exit\n\
      --version         output version information and exit\n\
"),
	  (separator_style == ss_braces ? _("braces") : _("space")));
  exit (0);
}

int
main (int argc, char **argv)
{
  program_name = argv[0];
  idh.idh_file_name = 0;

  /* Set locale according to user's wishes.  */
  setlocale (LC_ALL, "");

  /* Tell program which translations to use and where to find.  */
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  for (;;)
    {
      int optc = getopt_long (argc, argv, "f:F:a:k:R:S:ilrwsxdo",
			      long_options, (int *) 0);
      if (optc < 0)
	break;
      switch (optc)
	{
	case 0:
	  break;

	case 'f':
	  idh.idh_file_name = optarg;
	  break;

	case 'F':
	  parse_frequency_arg (optarg);
	  break;

	case 'a':
	  ambiguous_prefix_length = stoi (optarg);
	  break;

	case 'k':
	  key_style = parse_key_style (optarg);
	  break;

	case 'R':
	  result_style = parse_result_style (optarg);
	  break;

	case 'S':
	  separator_style = parse_separator_style (optarg);
	  break;

	case 'i':
	  ignore_case_flag = REG_ICASE;
	  break;

	case 'l':
	  pattern_style = ps_literal;
	  break;

	case 'r':
	  pattern_style = ps_regexp;
	  break;

	case 'e':
	  pattern_style = ps_regexp;
	  error (0, 0, _("notice: use of `-e' is deprecated, use `-r' instead"));
	  break;

	case 'w':
	  delimiter_style = ds_word;
	  break;

	case 's':
	  delimiter_style = ds_substring;
	  break;

	case 'x':
	  radix_flag |= radix_hex;
	  break;

	case 'd':
	  radix_flag |= radix_dec;
	  break;

	case 'o':
	  radix_flag |= radix_oct;
	  break;

	default:
	  usage ();
	}
    }

  if (show_version)
    {
      printf ("%s - %s\n", program_name, PACKAGE_VERSION);
      exit (0);
    }

  if (show_help)
    help_me ();

  if (separator_style == ss_contextual)
    {
      if (isatty (STDOUT_FILENO))
	separator_style = DEFAULT_SEPARATOR_STYLE;
      else if (key_style == ks_none)
	separator_style = ss_newline;
      else
	separator_style = ss_space;
    }

  argc -= optind;
  argv += optind;
  if (argc == 0)
    {
      static char dot[] = ".";
      static char *dotp = dot;
      argc = 1;
      argv = &dotp;
    }

  /* Look for the ID database up the tree */
  idh.idh_file_name = locate_id_file_name (idh.idh_file_name);
  if (idh.idh_file_name == 0)
    error (1, errno, _("can't locate `ID'"));

  init_idh_obstacks (&idh);
  init_idh_tables (&idh);

  cw_dlink = get_current_dir_link ();

  /* Determine absolute name of the directory name to which database
     constituent files are relative. */
  members_0 = read_id_file (idh.idh_file_name, &idh);
  bits_vec_size = (idh.idh_files + 7) / 4; /* more than enough */
  tree8_levels = tree8_count_levels (idh.idh_files);

  hits_buf_1 = MALLOC (char, idh.idh_buf_size);
  hits_buf_2 = MALLOC (char, idh.idh_buf_size);
  bits_vec = MALLOC (unsigned char, bits_vec_size);

  report_function = get_report_func ();
  if (ambiguous_prefix_length)
    {
      if (!query_ambiguous_prefix (ambiguous_prefix_length, report_function))
	fprintf (stderr, _("All identifiers are non-ambiguous within the first %d characters\n"),
		 ambiguous_prefix_length);
    }
  else
    {
      while (argc)
	{
	  char *pattern = (argc--, *argv++);
	  if (ignore_case_flag)
	    lower_caseify (pattern);
	  query_function = get_query_func (pattern);
	  (*query_function) (pattern, report_function);
	}
    }

  fclose (idh.idh_FILE);
  exit (0);
}

void
lower_caseify (char *str)
{
  while (*str)
    {
      *str = TOLOWER (*str);
      str++;
    }
}

enum key_style
parse_key_style (char const *arg)
{
  MAYBE_RETURN_PREFIX_MATCH (arg, "none", ks_none);
  MAYBE_RETURN_PREFIX_MATCH (arg, "token", ks_token);
  MAYBE_RETURN_PREFIX_MATCH (arg, "pattern", ks_pattern);
  error (0, 0, _("invalid `--key' style: `%s'"), arg);
  usage ();
  return ks_bogus;
}

enum result_style
parse_result_style (char const *arg)
{
  MAYBE_RETURN_PREFIX_MATCH (arg, "none", rs_none);
  MAYBE_RETURN_PREFIX_MATCH (arg, "filenames", rs_filenames);
  MAYBE_RETURN_PREFIX_MATCH (arg, "grep", rs_grep);
  MAYBE_RETURN_PREFIX_MATCH (arg, "edit", rs_edit);
  error (0, 0, _("invalid `--result' style: `%s'"), arg);
  usage ();
  return rs_bogus;
}

query_func_t
get_query_func (char *pattern)
{
  switch (pattern_style)
    {
    case ps_regexp:
      return query_regexp;

    case ps_literal:
      if (delimiter_style == ds_substring)
	return query_literal_substring;
      else
	return query_literal_word;

    default:
      if (is_regexp (pattern))
	return query_regexp;
      else if (has_left_delimiter (pattern))
	return query_literal_prefix;
      else if (delimiter_style == ds_substring)
	return query_literal_substring;
      else if (is_number (pattern))
	return query_number;
      else if (delimiter_style == ds_word)
	return query_literal_word;
      else
	return query_literal_word;
    }
}

report_func_t
get_report_func (void)
{
  switch (result_style)
    {
    case rs_filenames: return report_filenames;
    case rs_grep: return report_grep;
    case rs_edit: return report_edit;
    default: return report_nothing;
    }
}

void
report_filenames (char const *name, struct file_link **flinkv)
{
  if (name && key_style != ks_none)
    printf ("%-14s ", name);
  print_filenames (flinkv, separator_style);
}

void
report_grep (char const *name, struct file_link **flinkv)
{
  char line[1<<020];
  char const *pattern = 0;
  regex_t compiled;
  char *file_name = ALLOCA (char, PATH_MAX);

  if (key_style == ks_pattern)
    {
      pattern = file_regexp (name, "[^a-zA-Z0-9_\300-\377]_*", "[^a-zA-Z0-9_\300-\377]");
      if (pattern)
	{
	  int regcomp_errno = regcomp (&compiled, pattern,
				       ignore_case_flag | REG_EXTENDED);
	  if (regcomp_errno)
	    {
	      char buf[BUFSIZ];
	      regerror (regcomp_errno, &compiled, buf, sizeof (buf));
	      error (1, 0, "%s", buf);
	    }
	}
    }

  line[0] = ' ';		/* sentinel */
  while (*flinkv)
    {
      int line_number = 0;
      FILE *gid_FILE;

      maybe_relative_file_name (file_name, *flinkv++, cw_dlink);
      gid_FILE = fopen (file_name, "r");
      if (gid_FILE == 0)
	{
	  error (0, errno, "can't open `%s'", file_name);
	  continue;
	}

      while (fgets (line + 1, sizeof (line) - 1, gid_FILE))
	{
	  line_number++;
	  if (pattern)
	    {
	      int regexec_errno = regexec (&compiled, line, 0, 0, 0);
	      if (regexec_errno == REG_ESPACE)
		error (1, 0, "can't match regular-expression: memory exhausted");
	      else if (regexec_errno)
		continue;
	    }
	  else if (word_match (name, line))
	    printf ("%s:%d:%s", file_name, line_number, line + 1);
	}
      fclose (gid_FILE);
    }
}

#ifndef WIN32
void
report_edit (char const *name, struct file_link **flinkv)
{
  static char const *editor;
  static char const *eid_arg;
  static char const *eid_right_del;
  static char const *eid_left_del;
  char regexp_buf[BUFSIZ];
  char ed_arg_buffer[BUFSIZ];
  char const *pattern;
  int c;
  int skip;

  if (editor == 0)
    {
      editor = getenv ("VISUAL");
      if (editor == 0)
	{
	  editor = getenv ("EDITOR");
	  if (editor == 0)
	    editor = "vi";
	}
    }

  if (eid_arg == 0)
    {
      int using_vi = strequ ("vi", basename (editor));

      eid_arg = getenv ("EIDARG");
      if (eid_arg == 0)
	eid_arg = (using_vi ? "+1;/%s/" : "");

      eid_left_del = getenv ("EIDLDEL");
      if (eid_left_del == 0)
	eid_left_del = (using_vi ? "\\<" : "");

      eid_right_del = getenv ("EIDRDEL");
      if (eid_right_del == 0)
	eid_right_del = (using_vi ? "\\>" : "");
    }

  report_filenames (name, flinkv);
  savetty ();
  for (;;)
    {
      /* FIXME: i18n of responses */
      printf (_("edit? [y1-9^S/nq] "));
      fflush (stdout);
      chartty ();
      c = (getchar () & 0177);
      restoretty ();
      switch (TOLOWER (c))
	{
	case '/': case ('s' & 037):
	  putchar ('/');
	  skip = search_flinkv (flinkv);
	  if (skip < 0)
	    continue;
	  flinkv += skip;
	  goto editit;

	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	  putchar (c);
	  skip = c - '0';
	  break;

	case 'y':
	  putchar (c);
	  skip = 0;
	  break;

	case '\n':
	case '\r':
	  putchar ('y');
	  skip = 0;
	  break;

	case 'q':
	  putchar (c);
	  putchar ('\n');
	  exit (0);

	case 'n':
	  putchar (c);
	  putchar ('\n');
	  return;

	default:
	  putchar (c);
	  putchar ('\n');
	  continue;
	}

      putchar ('\n');
      while (skip--)
	if (*++flinkv == 0)
	  continue;
      break;
    }
editit:

  if (key_style == ks_pattern)
    pattern = file_regexp (name, eid_left_del, eid_right_del);
  else
    pattern = 0;
  if (pattern == 0)
    {
      pattern = regexp_buf;
      sprintf (regexp_buf, "%s%s%s", eid_left_del, name, eid_right_del);
    }

  switch (fork ())
    {
    case -1:
      error (1, errno, _("can't fork"));
      break;

    case 0:
      {
	char **argv_0 = MALLOC (char *, 3 + vector_cardinality (flinkv));
	char **argv = argv_0 + 2;
	while (*flinkv)
	  *argv++ = maybe_relative_file_name (0, *flinkv++, cw_dlink);
	*argv = 0;
	argv = argv_0 + 1;
	if (eid_arg)
	  {
	    sprintf (ed_arg_buffer, eid_arg, pattern);
	    *argv-- = ed_arg_buffer;
	  }
	*(char const **) argv = editor;
	execvp (editor, argv);
	error (0, errno, _("can't exec `%s'"), editor);
      }

    default:
      {
	void (*oldint) __P((int)) = signal (SIGINT, SIG_IGN);
	void (*oldquit) __P((int)) = signal (SIGQUIT, SIG_IGN);

	while (wait (0) == -1 && errno == EINTR)
	  ;

	signal (SIGINT, oldint);
	signal (SIGQUIT, oldquit);
      }
      break;
    }
}
#else //WIN32
void report_edit (char const *name, struct file_link **flinkv)
{
}

#endif //WIN32

void
report_nothing (char const *name, struct file_link **flinkv)
{
  if (key_style != ks_none)
    puts (name);
}

int
vector_cardinality (void *vector)
{
  void **v = (void **) vector;
  int count = 0;

  while (*v++)
    count++;
  return count;
}

int
search_flinkv (struct file_link **flinkv)
{
  char pattern[BUFSIZ];
  unsigned int count;
  char *file_name = ALLOCA (char, PATH_MAX);

  if (fgets (pattern, sizeof (pattern), stdin) == 0)
    return -1;

  for (count = 0; *flinkv; count++, flinkv++)
    {
      maybe_relative_file_name (file_name, *flinkv, cw_dlink);
      if (strcasestr (file_name, pattern))
	return count;
    }
  return -1;
}

int
query_literal_word (char const *arg, report_func_t report_func)
{
  if (query_binary_search (arg) == 0)
    return 0;
  gets_past_00 (hits_buf_1, idh.idh_FILE);
  assert (*hits_buf_1);
  if (!desired_frequency (hits_buf_1))
    return 0;
  (*report_func) (hits_buf_1, tree8_to_flinkv (token_hits_addr (hits_buf_1)));
  return 1;
}

int
query_literal_prefix (char const *arg, report_func_t report_func)
{
  int count;
  unsigned int length;

  if (query_binary_search (++arg) == 0)
    return 0;

  length = strlen (arg);
  count = 0;
  if (key_style != ks_token)
    memset (bits_vec, 0, bits_vec_size);
  while (gets_past_00 (hits_buf_1, idh.idh_FILE) > 0)
    {
      assert (*hits_buf_1);
      if (!desired_frequency (hits_buf_1))
	continue;
      if (!strnequ (arg, hits_buf_1, length))
	break;
      if (key_style == ks_token)
	(*report_func) (hits_buf_1, tree8_to_flinkv (token_hits_addr (hits_buf_1)));
      else
	tree8_to_bits (bits_vec, token_hits_addr (hits_buf_1));
      count++;
    }
  if (key_style != ks_token && count)
    (*report_func) (--arg, bits_to_flinkv (bits_vec));

  return count;
}

int
query_regexp (char const *pattern_0, report_func_t report_func)
{
  int count;
  regex_t compiled;
  int regcomp_errno;
  char const *pattern = pattern_0;

  pattern = maybe_add_word_delimiters (pattern);
  regcomp_errno = regcomp (&compiled, pattern,
			   ignore_case_flag | REG_EXTENDED);
  if (regcomp_errno)
    {
      char buf[BUFSIZ];
      regerror (regcomp_errno, &compiled, buf, sizeof (buf));
      error (1, 0, "%s", buf);
    }
  fseek (idh.idh_FILE, idh.idh_tokens_offset, SEEK_SET);

  count = 0;
  if (key_style != ks_token)
    memset (bits_vec, 0, bits_vec_size);
  while (gets_past_00 (hits_buf_1, idh.idh_FILE) > 0)
    {
      int regexec_errno;
      assert (*hits_buf_1);
      if (!desired_frequency (hits_buf_1))
	continue;
      regexec_errno = regexec (&compiled, hits_buf_1, 0, 0, 0);
      if (regexec_errno == REG_ESPACE)
	error (0, 0, _("can't match regular-expression: memory exhausted"));
      else if (regexec_errno)
	continue;
      if (key_style == ks_token)
	(*report_func) (hits_buf_1, tree8_to_flinkv (token_hits_addr (hits_buf_1)));
      else
	tree8_to_bits (bits_vec, token_hits_addr (hits_buf_1));
      count++;
    }
  if (key_style != ks_token && count)
    (*report_func) (pattern, bits_to_flinkv (bits_vec));

  if (pattern != pattern_0)
    free ((char *) pattern);

  return count;
}

char const *
maybe_add_word_delimiters (char const *pattern_0)
{
  if (delimiter_style != ds_word)
    return pattern_0;
  else
    {
      int length = strlen (pattern_0);
      int has_left = has_left_delimiter (pattern_0);
      int has_right = has_right_delimiter (&pattern_0[length]);
      if (has_left && has_right)
	return pattern_0;
      else
	{
	  char *pattern = MALLOC (char, length + 4);
	  if (has_left)
	    strcpy (pattern, pattern_0);
	  else
	    {
	      length += 2;
	      strcpy (pattern, "\\<");
	      strcpy (pattern + 2, pattern_0);
	    }
	  if (!has_right)
	    strcpy (pattern + length, "\\>");
	  return pattern;
	}
    }
}

int
query_number (char const *arg, report_func_t report_func)
{
  int count;
  int radix;
  int val;
  int hit_digits = 0;

  radix = (val = stoi (arg)) ? radix_all : get_radix (arg);
  fseek (idh.idh_FILE, idh.idh_tokens_offset, SEEK_SET);

  count = 0;
  if (key_style != ks_token)
    memset (bits_vec, 0, bits_vec_size);
  while (gets_past_00 (hits_buf_1, idh.idh_FILE) > 0)
    {
      if (hit_digits)
	{
	  if (!isdigit (*hits_buf_1))
	    break;
	}
      else
	{
	  if (isdigit (*hits_buf_1))
	    hit_digits = 1;
	}

      if (!((radix_flag ? radix_flag : radix) & get_radix (hits_buf_1))
	  || stoi (hits_buf_1) != val)
	continue;
      if (key_style == ks_token)
	(*report_func) (hits_buf_1, tree8_to_flinkv (token_hits_addr (hits_buf_1)));
      else
	tree8_to_bits (bits_vec, token_hits_addr (hits_buf_1));
      count++;
    }
  if (key_style != ks_token && count)
    (*report_func) (arg, bits_to_flinkv (bits_vec));

  return count;
}

/* Find identifiers that are non-unique within the first `count'
   characters.  */

int
query_ambiguous_prefix (unsigned int limit, report_func_t report_func)
{
  char *old = hits_buf_1;
  char *new = hits_buf_2;
  int consecutive = 0;
  int count = 0;
  char name[1024];

  if (limit <= 1)
    usage ();
  assert (limit < sizeof(name));

  name[0] = '^';
  *new = '\0';
  fseek (idh.idh_FILE, idh.idh_tokens_offset, SEEK_SET);
  while (gets_past_00 (old, idh.idh_FILE) > 0)
    {
      char *tmp;
      if (!(token_flags (old) & TOK_NAME))
	continue;
      tmp = old;
      old = new;
      new = tmp;
      if (!strnequ (new, old, limit))
	{
	  if (consecutive && key_style != ks_token)
	    {
	      strncpy (&name[1], old, limit);
	      (*report_func) (name, bits_to_flinkv (bits_vec));
	    }
	  consecutive = 0;
	  continue;
	}
      if (!consecutive++)
	{
	  if (key_style != ks_token)
	    tree8_to_bits (bits_vec, token_hits_addr (old));
	  else
	    (*report_func) (old, tree8_to_flinkv (token_hits_addr (old)));
	  count++;
	}
      if (key_style == ks_token)
	(*report_func) (new, tree8_to_flinkv (token_hits_addr (new)));
      else
	tree8_to_bits (bits_vec, token_hits_addr (new));
      count++;
    }
  if (consecutive && key_style != ks_token)
    {
      strncpy (&name[1], new, limit);
      (*report_func) (name, bits_to_flinkv (bits_vec));
    }
  return count;
}

int
query_literal_substring (char const *arg, report_func_t report_func)
{
  int count;
  char *(*strstr_func) __P((char const *, char const *));

  fseek (idh.idh_FILE, idh.idh_tokens_offset, SEEK_SET);

  count = 0;
  if (key_style != ks_token)
    memset (bits_vec, 0, bits_vec_size);
  strstr_func = (ignore_case_flag ? strcasestr : strstr);
  while (gets_past_00 (hits_buf_1, idh.idh_FILE) > 0)
    {
      assert (*hits_buf_1);
      if (!desired_frequency (hits_buf_1))
	continue;
      if ((*strstr_func) (hits_buf_1, arg) == 0)
	continue;
      if (key_style == ks_token)
	(*report_func) (hits_buf_1, tree8_to_flinkv (token_hits_addr (hits_buf_1)));
      else
	tree8_to_bits (bits_vec, token_hits_addr (hits_buf_1));
      count++;
    }
  if (key_style != ks_token && count)
    (*report_func) (arg, bits_to_flinkv (bits_vec));

  return count;
}

void
parse_frequency_arg (char const *arg)
{
  if (strnequ (arg, "..", 2))
    frequency_low = 1;
  else
    {
      frequency_low = atoi (arg);
      while (isdigit (*arg))
	arg++;
      if (strnequ (arg, "..", 2))
	arg += 2;
    }
  if (*arg)
    frequency_high = atoi (arg);
  else if (strnequ (&arg[-1], "..", 2))
    frequency_high = USHRT_MAX;
  else
    frequency_high = frequency_low;
  if (frequency_low > frequency_high)
    {
      unsigned int tmp = frequency_low;
      frequency_low = frequency_high;
      frequency_high = tmp;
    }
}

int
desired_frequency (char const *tok)
{
  unsigned int count = token_count (tok);
  return (frequency_low <= count && count <= frequency_high);
}

/* if string `s2' occurs in `s1', return a pointer to the first match.
   Ignore differences in alphabetic case.  */

char *
strcasestr (char const *s1, char const *s2)
{
  char const *s1p;
  char const *s2p;
  char const *s1last;

  for (s1last = &s1[strlen (s1) - strlen (s2)]; s1 <= s1last; s1++)
    for (s1p = s1, s2p = s2; TOLOWER (*s1p) == TOLOWER (*s2p); s1p++)
      if (*++s2p == '\0')
	return (char *) s1;
  return 0;
}

/* Convert the regular expression that we used to locate identifiers
   in the id database into one suitable for locating the identifiers
   in files.  */

char const *
file_regexp (char const *name_0, char const *left_delimit, char const *right_delimit)
{
  static char pat_buf[BUFSIZ];
  char *name = (char *) name_0;

  if (query_function == query_number && key_style == ks_pattern)
    {
      sprintf (pat_buf, "%s0*[Xx]*0*%d[Ll]*%s", left_delimit, stoi (name), right_delimit);
      return pat_buf;
    }

  if (!is_regexp (name) && name[0] != '^')
    return 0;

  if (name[0] == '^')
    name_0++;
  else
    left_delimit = "";
  while (*++name)
    ;
  if (*--name == '$')
    *name = '\0';
  else
    right_delimit = "";

  sprintf (pat_buf, "%s%s%s", left_delimit, name_0, right_delimit);
  return pat_buf;
}

off_t
query_binary_search (char const *token_0)
{
  off_t offset = 0;
  off_t start = idh.idh_tokens_offset - 2;
  off_t end = idh.idh_end_offset;
  off_t anchor_offset = 0;
  int order = -1;

  while (start < end)
    {
      int c;
      int incr = 1;
      char const *token;

      offset = start + (end - start) / 2;
      fseek (idh.idh_FILE, offset, SEEK_SET);
      offset += skip_past_00 (idh.idh_FILE);
      if (offset >= end)
	{
	  offset = start + 2;
	  fseek (idh.idh_FILE, offset, SEEK_SET);
	}

      /* compare the token names */
      token = token_0;
      while (*token == (c = getc (idh.idh_FILE)) && *token && c)
	{
	  token++;
	  incr++;
	}
      if (c && !*token && query_function == query_literal_prefix)
	anchor_offset = offset;
      order = *token - c;

      if (order < 0)
	end = offset - 2;
      else if (order > 0)
	start = offset + incr + skip_past_00 (idh.idh_FILE) - 2;
      else
	break;
    }

  if (order)
    {
      if (anchor_offset)
	offset = anchor_offset;
      else
	return 0;
    }
  fseek (idh.idh_FILE, offset, SEEK_SET);
  return offset;
}

/* Are there any regexp meta-characters in name?? */

int
is_regexp (char *name)
{
  int backslash = 0;

  if (*name == '^')
    name++;
  else if (strnequ (name, "\\<", 2))
    name += 2;
  while (*name)
    {
      if (*name == '\\')
	{
	  if (strchr ("<>", name[1]))
	    return 1;
	  name++, backslash++;
	}
      else if (strchr ("[]().*+^$", *name))
	return 1;
      name++;
    }
  if (backslash)
    while (*name)
      {
	if (*name == '\\')
	  strcpy (name, name + 1);
	name++;
      }
  return 0;
}

int
has_left_delimiter (char const *pattern)
{
  return (*pattern == '^' || strnequ (pattern, "\\<", 2));
}

int
has_right_delimiter (char const *pattern)
{
  return (pattern[-1] == '$' || strequ (pattern - 2, "\\>"));
}

/* file_name_wildcard implements a simple pattern matcher that
   emulates the shell wild card capability.

   * - any string of chars
   ? - any char
   [] - any char in set (if first char is !, any not in set)
   \ - literal match next char */

int
file_name_wildcard (char const *pattern, char const *fn)
{
  int c;
  int i;
  char set[256];
  int revset;

  while ((c = *pattern++) != '\0')
    {
      if (c == '*')
	{
	  if (*pattern == '\0')
	    return 1;		/* match anything at end */
	  while (*fn != '\0')
	    {
	      if (file_name_wildcard (pattern, fn))
		return 1;
	      ++fn;
	    }
	  return 0;
	}
      else if (c == '?')
	{
	  if (*fn++ == '\0')
	    return 0;
	}
      else if (c == '[')
	{
	  c = *pattern++;
	  memset (set, 0, 256);
	  if (c == '!')
	    {
	      revset = 1;
	      c = *pattern++;
	    }
	  else
	    revset = 0;
	  while (c != ']')
	    {
	      if (c == '\\')
		c = *pattern++;
	      set[c] = 1;
	      if ((*pattern == '-') && (*(pattern + 1) != ']'))
		{
		  pattern += 1;
		  while (++c <= *pattern)
		    set[c] = 1;
		  ++pattern;
		}
	      c = *pattern++;
	    }
	  if (revset)
	    for (i = 1; i < 256; ++i)
	      set[i] = !set[i];
	  if (!set[(int) *fn++])
	    return 0;
	}
      else
	{
	  if (c == '\\')
	    c = *pattern++;
	  if (c != *fn++)
	    return 0;
	}
    }
  return (*fn == '\0');
}

/* Does `name' occur in `line' delimited by non-alphanumerics?? */

int
word_match (char const *name_0, char const *line)
{
  char const *name = name_0;

  for (;;)
    {
      /* find an initial-character match */
      while (*line != *name)
	{
	  if (*line == '\0' || *line == '\n')
	    return 0;
	  line++;
	}
      /* do we have a word delimiter on the left ?? */
      if (IS_ALNUM (line[-1]))
	{
	  line++;
	  continue;
	}
      /* march down both strings as long as we match */
      while (*++name == *++line)
	;
      /* is this the end of `name', is there a word delimiter ?? */
      if (*name == '\0' && !IS_ALNUM (*line))
	return 1;
      name = name_0;
    }
}

/* Use the C lexical rules to determine an ascii number's radix.  The
   radix is returned as a bit map, so that more than one radix may
   apply.  In particular, it is impossible to determine the radix of
   0, so return all possibilities.  */

int
get_radix (char const *str)
{
  if (!isdigit (*str))
    return 0;
  if (*str != '0')
    return radix_dec;
  str++;
  if (*str == 'x' || *str == 'X')
    return radix_hex;
  while (*str && *str == '0')
    str++;
  return (*str ? radix_oct : (radix_oct | radix_dec));
}

int
is_number (char const *str)
{
  if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
      str += 2;
      str += strspn (str, "0123456789aAbBcCdDeEfF");
    }
  else
    str += strspn (str, "0123456789");
  str += strspn (str, "uUlL");
  return (*str == '\0');
}

/* Convert an ascii string number to an integer.  Determine the radix
   before converting.  */

int
stoi (char const *str)
{
  switch (get_radix (str))
    {
    case radix_dec:
      return (dtoi (str));
    case radix_oct:
      return (otoi (&str[1]));
    case radix_hex:
      return (xtoi (&str[2]));
    case radix_dec | radix_oct:
      return 0;
    default:
      return -1;
    }
}

/* Convert an ascii octal number to an integer. */

int
otoi (char const *str)
{
  int n = 0;

  while (*str >= '0' && *str <= '7')
    {
      n *= 010;
      n += *str++ - '0';
    }
  while (*str && strchr ("uUlL", *str))
    str++;
  return (*str ? -1 : n);
}

/* Convert an ascii decimal number to an integer. */

int
dtoi (char const *str)
{
  int n = 0;

  while (isdigit (*str))
    {
      n *= 10;
      n += *str++ - '0';
    }
  while (*str && strchr ("uUlL", *str))
    str++;
  return (*str ? -1 : n);
}

/* Convert an ascii hex number to an integer. */

int
xtoi (char const *str)
{
  int n = 0;

  while (isxdigit (*str))
    {
      n *= 0x10;
      if (isdigit (*str))
	n += *str++ - '0';
      else if (islower (*str))
	n += 0xa + *str++ - 'a';
      else
	n += 0xA + *str++ - 'A';
    }
  while (*str && strchr ("uUlL", *str))
    str++;
  return (*str ? -1 : n);
}

unsigned char *
tree8_to_bits (unsigned char *bv_0, unsigned char const *hits_tree8)
{
  unsigned char* bv = bv_0;
  tree8_to_bits_1 (&bv, &hits_tree8, tree8_levels);
  return bv_0;
}

void
tree8_to_bits_1 (unsigned char **bv, unsigned char const **hits_tree8, int level)
{
  int hits = *(*hits_tree8)++;

  if (--level)
    {
      int incr = 1 << ((level - 1) * 3);
      int bit;
      for (bit = 1; bit & 0xff; bit <<= 1)
	{
	  if (bit & hits)
	    tree8_to_bits_1 (bv, hits_tree8, level);
	  else
	    *bv += incr;
	}
    }
  else
    *(*bv)++ |= hits;
}

struct file_link **
bits_to_flinkv (unsigned char const *bv)
{
  int const reserved_flinkv_slots = 3;
  static struct file_link **flinkv_0;
  struct file_link **flinkv;
  struct file_link **members = members_0;
  struct file_link **end = &members_0[idh.idh_files];

  if (flinkv_0 == 0)
    flinkv_0 = MALLOC (struct file_link *, idh.idh_files + reserved_flinkv_slots + 2);
  flinkv = &flinkv_0[reserved_flinkv_slots];

  for (;;)
    {
      int hits;
      int bit;

      while (*bv == 0)
	{
	  bv++;
	  members += 8;
	  if (members >= end)
	    goto out;
	}
      hits = *bv++;
      for (bit = 1; bit & 0xff; bit <<= 1)
	{
	  if (bit & hits)
	    *flinkv++ = *members;
	  if (++members >= end)
	    goto out;
	}
    }
out:
  *flinkv = 0;
  return &flinkv_0[reserved_flinkv_slots];
}

struct file_link **
tree8_to_flinkv (unsigned char const *hits_tree8)
{
  memset (bits_vec, 0, bits_vec_size);
  return bits_to_flinkv (tree8_to_bits (bits_vec, hits_tree8));
}

#if HAVE_TERMIOS_H

#include <termios.h>
struct termios linemode;
struct termios charmode;
struct termios savemode;
#define GET_TTY_MODES(modes) tcgetattr (0, (modes))
#define SET_TTY_MODES(modes) tcsetattr(0, TCSANOW, (modes))

#else /* not HAVE_TERMIOS_H */

# if HAVE_SYS_IOCTL_H
#  include <sys/ioctl.h>
# endif

# if HAVE_TERMIO_H

#  include <termio.h>
struct termio linemode;
struct termio charmode;
struct termio savemode;
#define GET_TTY_MODES(modes) ioctl (0, TCGETA, (modes))
#define SET_TTY_MODES(modes) ioctl (0, TCSETA, (modes))

# else /* not HAVE_TERMIO_H */

#  if HAVE_SGTTY_H

#   include <sgtty.h>
struct sgttyb linemode;
struct sgttyb charmode;
struct sgttyb savemode;

#   ifdef TIOCGETP
#define GET_TTY_MODES(modes) ioctl (0, TIOCGETP, (modes))
#define SET_TTY_MODES(modes) ioctl (0, TIOCSETP, (modes))
#   else
#define GET_TTY_MODES(modes) gtty (0, (modes))
#define SET_TTY_MODES(modes) stty (0, (modes))
#   endif

#  else /* not HAVE_SGTTY_H */

#define GET_TTY_MODES(modes)
#define SET_TTY_MODES(modes)

#  endif /* not HAVE_SGTTY_H */
# endif /* not HAVE_TERMIO_H */
#endif /* not HAVE_TERMIOS_H */

#if HAVE_TERMIOS_H || HAVE_TERMIO_H

void
savetty (void)
{
  GET_TTY_MODES (&savemode);
  charmode = linemode = savemode;

  charmode.c_lflag &= ~(ECHO | ICANON | ISIG);
  charmode.c_cc[VMIN] = 1;
  charmode.c_cc[VTIME] = 0;

  linemode.c_lflag |= (ECHO | ICANON | ISIG);
  linemode.c_cc[VEOF] = 'd' & 037;
  linemode.c_cc[VEOL] = 0377;
}

#else /* not (HAVE_TERMIOS_H || HAVE_TERMIO_H) */

# if HAVE_SGTTY_H

void
savetty (void)
{
#  ifdef TIOCGETP
  ioctl(0, TIOCGETP, &savemode);
#  else
  gtty(0, &savemode);
#  endif
  charmode = linemode = savemode;

  charmode.sg_flags &= ~ECHO;
  charmode.sg_flags |= RAW;

  linemode.sg_flags |= ECHO;
  linemode.sg_flags &= ~RAW;
}

# else /* not HAVE_SGTTY_H */

void
savetty (void)
{
}

# endif /* not HAVE_SGTTY_H */
#endif /* not (HAVE_TERMIOS_H || HAVE_TERMIO_H) */

void
restoretty (void)
{
  SET_TTY_MODES (&savemode);
}

void
linetty (void)
{
  SET_TTY_MODES (&linemode);
}

void
chartty (void)
{
  SET_TTY_MODES (&charmode);
}
