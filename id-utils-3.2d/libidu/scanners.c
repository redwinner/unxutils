/* scanners.c -- file & directory name manipulations
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
#if HAVE_FCNTL_H
# include <fcntl.h>
#endif
#include <getopt.h>
#include "xstdlib.h"
#include "xstddef.h"
#include "xunistd.h"
#include "xsysstat.h"
#include "xstring.h"
#include "xmalloc.h"
#include "xnls.h"
#include "error.h"
#include "scanners.h"
#include "tokflags.h"

#define DEBUG(args) /* printf args */

struct obstack lang_args_obstack;
struct lang_args *lang_args_default = 0;
struct lang_args *lang_args_list = 0;
struct obstack tokens_obstack;
int log_8_member_files = 0;

extern void usage __P((void));
extern char *program_name;

/****************************************************************************/

struct lang_args **parse_language_map_file __P((char const *file_name, struct lang_args **next_ptr));
char *read_language_map_file __P((char const *file_name));
void tokenize_args_string __P((char *args_string, int *argcp, char ***argvp));

static struct token *get_token_c __P((FILE *in_FILE, void const *args, int *flags));
static void *parse_args_c __P((char **argv, int argc));
static void help_me_c __P((void));

static struct token *get_token_asm __P((FILE *in_FILE, void const *args, int *flags));
static void *parse_args_asm __P((char **argv, int argc));
static void help_me_asm __P((void));

static struct token *get_token_text __P((FILE *in_FILE, void const *args, int *flags));
static void *parse_args_text __P((char **argv, int argc));
static void help_me_text __P((void));

struct language languages_0[] =
{
  { "C", parse_args_c, get_token_c, help_me_c },
  { "C++", parse_args_c, get_token_c, help_me_c },
  { "asm", parse_args_asm, get_token_asm, help_me_asm },
  { "text", parse_args_text, get_token_text, help_me_text },
};
struct language const *languages_N = &languages_0[cardinalityof (languages_0)];

void
language_help_me (void)
{
  struct language *lang;
  for (lang = languages_0; lang < languages_N; lang++)
    {
      putchar ('\n');
      (*lang->lg_help_me) ();
    }
}

void
language_save_arg (char *arg)
{
  static char horizontal_space[] = " \t";
  char *lang_name = strtok (arg, ":");
  struct language *lang = get_language (lang_name);

  if (lang == 0)
    {
      error (0, 0, _("unrecognized language: `%s'"), lang_name);
      usage ();
    }
  if (lang->lg_argc == 0)
    lang->lg_argv[lang->lg_argc++] = program_name;
  lang->lg_argv[lang->lg_argc++] = strtok (0, horizontal_space);
}

void
language_getopt (void)
{
  struct language *lang;

  for (lang = languages_0; lang < languages_N; lang++)
    if (lang->lg_argc)
      lang->lg_parse_args (lang->lg_argv, lang->lg_argc);
}

struct language *
get_language (char const *lang_name)
{
  struct language *lang;

  for (lang = languages_0; lang < languages_N; lang++)
    if (strequ (lang_name, lang->lg_name))
      {
	DEBUG (("lang=%s", lang_name));
	return lang;
      }
  DEBUG (("!lang=%s", lang_name));
  return 0;
}

/****************************************************************************/

int lang_args_index = 0;

void
set_default_language (char const *lang_name)
{
}

void
parse_language_map (char const *file_name)
{
  if (obstack_init (&lang_args_obstack) == 0)
    error (1, 0, _("can't allocate language args obstack: memory exhausted"));
  if (file_name == 0)
    file_name = LANGUAGE_MAP_FILE;
  parse_language_map_file (file_name, &lang_args_list);
}

struct lang_args **
parse_language_map_file (char const *file_name, struct lang_args **next_ptr)
{
  static char white_space[] = " \t\r\n\v\f";
  static char horizontal_space[] = " \t";
  static char vertical_space[] = "\r\n\v\f";
  char *lang_map_buffer;
  char *lmp;

  lmp = lang_map_buffer = read_language_map_file (file_name);
  for (;;)
    {
      struct lang_args *new_args;
      struct language const *lang;
      int pattern_size;
      char *lang_name;
      int space;

      /* Skip leading white space and full-line comments */
      while (*lmp)
	{
	  lmp += strspn (lmp, white_space);
	  if (*lmp != '#')
	    break;
	  lmp += strcspn (lmp, vertical_space);
	}
      if (*lmp == '\0')
	break;

      pattern_size = strcspn (lmp, white_space);
      if (pattern_size == 3 && strnequ (lmp, "***", 3))
	{
	  lmp += pattern_size;
	  lmp += strspn (lmp, horizontal_space);
	  if (isspace (*lmp))
	    next_ptr = parse_language_map_file (LANGUAGE_MAP_FILE, next_ptr);
	  else
	    {
	      char *end = lmp + strcspn (lmp, white_space);
	      *end = '\0';
	      next_ptr = parse_language_map_file (lmp, next_ptr);
	      lmp = end + 1;
	    }
	  continue;
	}

      new_args = OBSTACK_ALLOC (&lang_args_obstack, struct lang_args, 1);
      if (new_args == 0)
	error (1, 0, _("can't allocate language args: memory exhausted"));
      new_args->la_pattern = obstack_copy0 (&lang_args_obstack, lmp, pattern_size);
      new_args->la_args_string = 0;
      new_args->la_next = 0;
      lmp += pattern_size;
      lmp += strspn (lmp, horizontal_space);
      if (isspace (*lmp))
	{
	  error (0, 0, _("language name expected following `%s' in file `%s'"),
		 new_args->la_pattern, file_name);
	  obstack_free (&lang_args_obstack, new_args);
	  continue;
	}
      lang_name = lmp;
      lmp += strcspn (lmp, white_space);
      space = *lmp;
      *lmp++ = '\0';
      lmp += strspn (lmp, horizontal_space);
      lang = new_args->la_language = get_language (lang_name);

      if (*lmp == '#')
	lmp += strcspn (lmp, vertical_space);
      else if (!isspace (*lmp) && (space == ' ' || space == '\t'))
	{
	  int args_size = strcspn (lmp, vertical_space);
	  new_args->la_args_string = obstack_copy0 (&lang_args_obstack, lmp, args_size);
	  lmp += args_size;
	}
      new_args->la_args_digested = (lang
				    ? lang->lg_parse_args (&new_args->la_args_string, 0)
				    : 0);
      if (pattern_size == 2 && strnequ (new_args->la_pattern, "**", 2))
	{
	  if (lang_args_default)
	    {
	      obstack_free (&lang_args_obstack, new_args);
	      continue;
	    }
	  lang_args_default = new_args;
	  DEBUG ((", <default>"));
	}
      else
	{
	  new_args->la_index = lang_args_index++;
	  *next_ptr = new_args;
	  next_ptr = &new_args->la_next;
	}
      DEBUG ((", pat=%s\n", new_args->la_pattern));
    }
  free (lang_map_buffer);
  return next_ptr;
}

char *
read_language_map_file (char const *file_name)
{
  int map_fd;
  char *lang_map_buffer;
  struct stat st;
  int bytes;

  map_fd = open (file_name, O_RDONLY);
  if (map_fd < 0)
    error (1, errno, _("can't open language map file `%s'"), file_name);
  if (fstat (map_fd, &st) < 0)
    error (1, errno, _("can't get size of map file `%s'"), file_name);

  lang_map_buffer = MALLOC (char, st.st_size + 2);
  if (lang_map_buffer == 0)
    error (1, 0, _("can't allocate language args: memory exhausted"));
  lang_map_buffer[st.st_size] = '\n';
  lang_map_buffer[st.st_size+1] = '\0';

  bytes = read (map_fd, lang_map_buffer, st.st_size);
  if (bytes < 0)
    error (1, errno, _("can't read language map file `%s'"), file_name);
  /* FIXME: handle interrupted & partial reads */
  if (bytes != st.st_size)
    error (1, errno, _("can't read entire language map file `%s'"), file_name);

  close (map_fd);
  return lang_map_buffer;
}

/****************************************************************************/

void
tokenize_args_string (char *args_string, int *argcp, char ***argvp)
{
  static char horizontal_space[] = " \t";
  char **argv_0 = MALLOC (char *, strlen (args_string) / 2);
  char **argv = argv_0;
  char *arg;

  *argv++ = program_name;
  arg = strtok (args_string, horizontal_space);
  while (arg)
    {
      *argv++ = arg;
      arg = strtok (0, horizontal_space);
    }
  *argcp = argv - argv_0;
  *argvp = REALLOC (argv_0, char *, *argcp);
}

static void
set_ushort_ctype (unsigned short *ctype, char const *chars, int type)
{
  unsigned short *rct = &ctype[1];
  unsigned char const *uc = (unsigned char const *) chars;

  while (*uc)
    rct[*uc++] |= type;
}

static void
clear_ushort_ctype (unsigned short *ctype, char const *chars, int type)
{
  unsigned short *rct = &ctype[1];
  unsigned char const *uc = (unsigned char const *) chars;

  while (*uc)
    rct[*uc++] &= ~type;
}

static void
set_uchar_ctype (unsigned char *ctype, char const *chars, int type)
{
  unsigned char *rct = &ctype[1];
  unsigned char const *uc = (unsigned char const *) chars;

  while (*uc)
    rct[*uc++] |= type;
}

static void
clear_uchar_ctype (unsigned char *ctype, char const *chars, int type)
{
  unsigned char *rct = &ctype[1];
  unsigned char const *uc = (unsigned char const *) chars;

  while (*uc)
    rct[*uc++] &= ~type;
}

/*************** C & C++ ****************************************************/

#define I1	0x0001		/* 1st char of an identifier [a-zA-Z_] */
#define DG	0x0002		/* decimal digit [0-9] */
#define NM	0x0004		/* extra chars in a hex or long number [a-fA-FxXlL] */
#define C1	0x0008		/* C comment introduction char: / */
#define C2	0x0010		/* C comment termination  char: * */
#define Q1	0x0020		/* single quote: ' */
#define Q2	0x0040		/* double quote: " */
#define ES	0x0080		/* escape char: \ */
#define NL	0x0100		/* newline: \n */
#define EF	0x0200		/* EOF */
#define SK	0x0400		/* Make these chars valid for names within strings */
#define VH	0x0800		/* VHIL comment introduction char: # */
#define WS	0x1000		/* White space characters */

/* character class membership macros: */

#define ISDIGIT(c)	((rct)[c] & (DG))	/* digit */
#define ISNUMBER(c)	((rct)[c] & (DG|NM))	/* legal in a number */
#define ISEOF(c)	((rct)[c] & (EF))	/* EOF */
#define ISID1ST(c)	((rct)[c] & (I1))	/* 1st char of an identifier */
#define ISIDREST(c)	((rct)[c] & (I1|DG))	/* rest of an identifier */
#define ISSTRKEEP(c)	((rct)[c] & (I1|DG|SK))	/* keep contents of string */
#define ISSPACE(c)	((rct)[c] & (WS))	/* white space character */

/* The `BORING' classes should be skipped over until something
   interesting comes along... */

#define ISBORING(c)	(!((rct)[c] & (EF|NL|I1|DG|Q1|Q2|C1|VH)))	/* fluff */
#define ISCBORING(c)	(!((rct)[c] & (EF|C2)))	/* comment fluff */
#define ISCCBORING(c)	(!((rct)[c] & (EF|NL)))	/* C++ // comment fluff */
#define ISQ1BORING(c)	(!((rct)[c] & (EF|NL|Q1|ES)))	/* char const fluff */
#define ISQ2BORING(c)	(!((rct)[c] & (EF|NL|Q2|ES)))	/* quoted str fluff */

static unsigned short ctype_c[257] =
{
  EF,
/*      0       1       2       3       4       5       6       7   */
/*    -----   -----   -----   -----   -----   -----   -----   ----- */
/*000*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*010*/ 0,	0,	NL,	0,	0,	0,	0,	0,
/*020*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*030*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*040*/ 0,	0,	Q2,	0,	0,	0,	0,	Q1,
/*050*/ 0,	0,	C2,	0,	0,	0,	0,	C1,
/*060*/ DG,	DG,	DG,	DG,	DG,	DG,	DG,	DG,
/*070*/ DG,	DG,	0,	0,	0,	0,	0,	0,
/*100*/ 0,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1,
/*110*/ I1,	I1,	I1,	I1,	I1|NM,	I1,	I1,	I1,
/*120*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*130*/ I1|NM,	I1,	I1,	0,	ES,	0,	0,	I1,
/*140*/ 0,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1,
/*150*/ I1,	I1,	I1,	I1,	I1|NM,	I1,	I1,	I1,
/*160*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*170*/ I1|NM,	I1,	I1,	0,	0,	0,	0,	0,
  /* FIXME: latin-1 */
};

struct args_c
{
  int strip_underscore;
  unsigned short *ctype;
};

static struct args_c args_c = { 0, ctype_c };

static struct option const long_options_c[] =
{
  { "keep", required_argument, 0, 'k' },
  { "ignore", required_argument, 0, 'i' },
  { "strip-underscore", no_argument, 0, 'u' },
  { 0 }
};

static void
help_me_c (void)
{
  printf (_("\
C language:\n\
  -k,--keep=CHARS        Allow CHARS in single-token strings, keep the result\n\
  -i,--ignore=CHARS      Allow CHARS in single-token strings, toss the result\n\
  -u,--strip-underscore  Strip a leading underscore from single-token strings\n\
"));
}

static void *
parse_args_c (char **argv, int argc)
{
  char *tmp_string = 0;
  struct args_c *args;

  if (argv == 0 || *argv == 0)
    return &args_c;

  if (argc)
    args = &args_c;
  else
    {
      tmp_string = strdup (*argv);
      tokenize_args_string (tmp_string, &argc, &argv);
      args = MALLOC (struct args_c, 1);
      args->strip_underscore = 0;
      args->ctype = ctype_c;
    }

  optind = 0;
  for (;;)
    {
      int optc = getopt_long (argc, argv, "k:i:u",
			      long_options_c, (int *) 0);
      if (optc < 0)
	break;
      if ((optc == 'k' || optc == 'i') && args->ctype == ctype_c)
	args->ctype = CLONE (ctype_c, unsigned short, cardinalityof (ctype_c));
      switch (optc)
	{
	case 'k':
	  set_ushort_ctype (args->ctype, optarg, SK);
	  break;

	case 'i':
	  clear_ushort_ctype (args->ctype, optarg, SK);
	  break;

	case 'u':
	  args->strip_underscore = 1;
	  break;

	default:
	  usage ();
	}
    }
  if (tmp_string)
    {
      free (argv);
      free (tmp_string);
    }
  return args;
}

unsigned char *scanner_buffer;

#define SCAN_CPP_DIRECTIVE						\
  do									\
    {									\
      c = getc (in_FILE);						\
      while (ISBORING (c))						\
	c = getc (in_FILE);						\
      if (!ISID1ST (c))							\
	goto next;							\
      id = scanner_buffer;						\
      *id++ = c;							\
      while (ISIDREST (c = getc (in_FILE)))				\
	*id++ = c;							\
      *id = '\0';							\
      if (strequ (scanner_buffer, "include"))				\
	{								\
	  while (c == ' ' || c == '\t')					\
	    c = getc (in_FILE);						\
	  if (c == '\n')						\
	    {								\
	      new_line = 1;						\
	      goto top;							\
	    }								\
	  id = scanner_buffer;						\
	  if (c == '"')							\
	    {								\
	      c = getc (in_FILE);					\
	      while (c != '\n' && c != EOF && c != '"')			\
		{							\
		  *id++ = c;						\
		  c = getc (in_FILE);					\
		}							\
	      *flags = TOK_STRING;					\
	    }								\
	  else if (c == '<')						\
	    {								\
	      c = getc (in_FILE);					\
	      while (c != '\n' && c != EOF && c != '>')			\
		{							\
		  *id++ = c;						\
		  c = getc (in_FILE);					\
		}							\
	      *flags = TOK_STRING;					\
	    }								\
	  else if (ISID1ST (c))						\
	    {								\
	      *id++ = c;						\
	      while (ISIDREST (c = getc (in_FILE)))			\
		*id++ = c;						\
	      *flags = TOK_NAME;					\
	    }								\
	  else								\
	    {								\
	      while (c != '\n' && c != EOF)				\
		c = getc (in_FILE);					\
	      new_line = 1;						\
	      goto top;							\
	    }								\
	  while (c != '\n' && c != EOF)					\
	    c = getc (in_FILE);						\
	  new_line = 1;							\
	  obstack_grow0 (&tokens_obstack, scanner_buffer,		\
			 id - scanner_buffer);				\
	  return (struct token *) obstack_finish (&tokens_obstack);	\
	}								\
      if (strnequ (scanner_buffer, "if", 2)				\
	  || strequ (scanner_buffer, "define")				\
	  || strequ (scanner_buffer, "elif")	/* ansi C */		\
	  || strequ (scanner_buffer, "undef"))				\
	goto next;							\
      while ((c != '\n') && (c != EOF))					\
	c = getc (in_FILE);						\
      new_line = 1;							\
      goto top;								\
  } while (0)


/* Grab the next identifier from the C source file.  This state
   machine is built for speed, not elegance.  */

static struct token *
get_token_c (FILE *in_FILE, void const *args, int *flags)
{
#define ARGS ((struct args_c const *) args)
  static int new_line = 1;
  unsigned short const *rct = &ARGS->ctype[1];
  unsigned char *id = scanner_buffer;
  int c;

  obstack_blank (&tokens_obstack, OFFSETOF_TOKEN_NAME);

top:
  c = getc (in_FILE);
  if (new_line)
    {
      new_line = 0;
      if (c != '#')
	goto next;
      SCAN_CPP_DIRECTIVE;
    }

next:
  while (ISBORING (c))
    c = getc (in_FILE);

  switch (c)
    {
    case '"':
      id = scanner_buffer;
      *id++ = c = getc (in_FILE);
      for (;;)
	{
	  while (ISQ2BORING (c))
	    *id++ = c = getc (in_FILE);
	  if (c == '\\')
	    {
	      *id++ = c = getc (in_FILE);
	      continue;
	    }
	  else if (c != '"')
	    goto next;
	  break;
	}
      *--id = '\0';
      id = scanner_buffer;
      while (ISSTRKEEP (*id))
	id++;
      if (*id || id == scanner_buffer)
	{
	  c = getc (in_FILE);
	  goto next;
	}
      *flags = TOK_STRING;
      if (ARGS->strip_underscore && scanner_buffer[0] == '_' && scanner_buffer[1])
	obstack_grow0 (&tokens_obstack, scanner_buffer + 1, id - scanner_buffer - 1);
      else
	obstack_grow0 (&tokens_obstack, scanner_buffer, id - scanner_buffer);
      return (struct token *) obstack_finish (&tokens_obstack);

    case '\'':
      c = getc (in_FILE);
      for (;;)
	{
	  while (ISQ1BORING (c))
	    c = getc (in_FILE);
	  if (c == '\\')
	    {
	      c = getc (in_FILE);
	      continue;
	    }
	  else if (c == '\'')
	    c = getc (in_FILE);
	  goto next;
	}

    case '/':
      c = getc (in_FILE);
      if (c == '/')
	{			/* Cope with C++ comment */
	  while (ISCCBORING (c))
	    c = getc (in_FILE);
	  new_line = 1;
	  goto top;
	}
      else if (c != '*')
	goto next;
      c = getc (in_FILE);
      for (;;)
	{
	  while (ISCBORING (c))
	    c = getc (in_FILE);
	  c = getc (in_FILE);
	  if (c == '/')
	    {
	      c = getc (in_FILE);
	      goto next;
	    }
	  else if (ISEOF (c))
	    {
	      new_line = 1;
	      obstack_free (&tokens_obstack, obstack_finish (&tokens_obstack));
	      return 0;
	    }
	}

    case '\n':
      new_line = 1;
      goto top;

    default:
      if (ISEOF (c))
	{
	  new_line = 1;
	  obstack_free (&tokens_obstack, obstack_finish (&tokens_obstack));
	  return 0;
	}
      id = scanner_buffer;
      *id++ = c;
      if (ISID1ST (c))
	{
	  *flags = TOK_NAME;
	  while (ISIDREST (c = getc (in_FILE)))
	    *id++ = c;
	}
      else if (ISDIGIT (c))
	{
	  *flags = TOK_NUMBER;
	  while (ISNUMBER (c = getc (in_FILE)))
	    *id++ = c;
	}
      else
	{
	  if (isprint (c))
	    fprintf (stderr, _("junk: `%c'"), c);
	  else
	    fprintf (stderr, _("junk: `\\%03o'"), c);
	}
      ungetc (c, in_FILE);
      *flags |= TOK_LITERAL;
      obstack_grow0 (&tokens_obstack, scanner_buffer, id - scanner_buffer);
      return (struct token *) obstack_finish (&tokens_obstack);
    }
#undef ARGS
}

#undef I1
#undef DG
#undef NM
#undef C1
#undef C2
#undef Q1
#undef Q2
#undef ES
#undef NL
#undef EF
#undef SK
#undef VH
#undef WS
#undef ISDIGIT
#undef ISNUMBER
#undef ISEOF
#undef ISID1ST
#undef ISIDREST
#undef ISSTRKEEP
#undef ISSPACE
#undef ISBORING
#undef ISCBORING
#undef ISCCBORING
#undef ISQ1BORING
#undef ISQ2BORING

/*************** Assembly ***************************************************/

#define I1	0x01		/* 1st char of an identifier [a-zA-Z_] */
#define NM	0x02		/* digit [0-9a-fA-FxX] */
#define NL	0x04		/* newline: \n */
#define CM	0x08		/* assembler comment char: usually # or | */
#define IG	0x10		/* ignore `identifiers' with these chars in them */
#define C1	0x20		/* C comment introduction char: / */
#define C2	0x40		/* C comment termination  char: * */
#define EF	0x80		/* EOF */

/* Assembly Language character classes */
#define ISID1ST(c)	((rct)[c] & (I1))
#define ISIDREST(c)	((rct)[c] & (I1|NM))
#define ISNUMBER(c)	((rct)[c] & (NM))
#define ISEOF(c)	((rct)[c] & (EF))
#define ISCOMMENT(c)	((rct)[c] & (CM))
#define ISBORING(c)	(!((rct)[c] & (EF|NL|I1|NM|CM|C1)))
#define ISCBORING(c)	(!((rct)[c] & (EF|NL)))
#define ISCCBORING(c)	(!((rct)[c] & (EF|C2)))
#define ISIGNORE(c)	((rct)[c] & (IG))

static unsigned char ctype_asm[257] =
{
  EF,
/*      0       1       2       3       4       5       6       7   */
/*    -----   -----   -----   -----   -----   -----   -----   ----- */
/*000*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*010*/ 0,	0,	NL,	0,	0,	0,	0,	0,
/*020*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*030*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*040*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*050*/ 0,	0,	C2,	0,	0,	0,	0,	C1,
/*060*/ NM,	NM,	NM,	NM,	NM,	NM,	NM,	NM,
/*070*/ NM,	NM,	0,	0,	0,	0,	0,	0,
/*100*/ 0,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1,
/*110*/ I1,	I1,	I1,	I1,	I1|NM,	I1,	I1,	I1,
/*120*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*130*/ I1|NM,	I1,	I1,	0,	0,	0,	0,	I1,
/*140*/ 0,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1,
/*150*/ I1,	I1,	I1,	I1,	I1|NM,	I1,	I1,	I1,
/*160*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*170*/ I1|NM,	I1,	I1,	0,	0,	0,	0,	0,

};

struct args_asm
{
  int handle_cpp;
  int strip_underscore;
  unsigned char *ctype;
};

static struct args_asm args_asm = { 1, 0, ctype_asm };

static struct option const long_options_asm[] =
{
  { "comment", required_argument, 0, 'c' },
  { "keep", required_argument, 0, 'k' },
  { "ignore", required_argument, 0, 'i' },
  { "strip-underscore", no_argument, 0, 'u' },
  { "no-cpp", no_argument, 0, 'p' },
  { 0 }
};

static void
help_me_asm (void)
{
  printf (_("\
Assembly language:\n\
  -c,--comment=CHARS     Any of CHARS starts a comment until end-of-line\n\
  -k,--keep=CHARS        Allow CHARS in tokens, and keep the result\n\
  -i,--ignore=CHARS      Allow CHARS in tokens, and toss the result\n\
  -u,--strip-underscore  Strip a leading underscore from tokens\n\
  -n,--no-cpp            Don't handle C pre-processor directives\n\
"));
}

static void *
parse_args_asm (char **argv, int argc)
{
  char *tmp_string = 0;
  struct args_asm *args;

  if (argv == 0 || *argv == 0)
    return &args_asm;

  if (argc)
    args = &args_asm;
  else
    {
      tmp_string = strdup (*argv);
      tokenize_args_string (tmp_string, &argc, &argv);
      args = MALLOC (struct args_asm, 1);
      args->strip_underscore = 0;
      args->ctype = ctype_asm;
      args->handle_cpp = 1;
    }

  optind = 0;
  for (;;)
    {
      int optc = getopt_long (argc, argv, "c:k:i:un",
			      long_options_asm, (int *) 0);
      if (optc < 0)
	break;
      if ((optc == 'k' || optc == 'i' || optc == 'c')
	  && args->ctype == ctype_asm)
	args->ctype = CLONE (ctype_asm, unsigned char, cardinalityof (ctype_asm));
      switch (optc)
	{
	case 'c':
	  set_uchar_ctype (args->ctype, optarg, CM);
	  break;

	case 'k':
	  set_uchar_ctype (args->ctype, optarg, I1);
	  break;

	case 'i':
	  set_uchar_ctype (args->ctype, optarg, I1 | IG);
	  break;

	case 'u':
	  args->strip_underscore = 1;
	  break;

	case 'n':
	  args->handle_cpp = 0;
	  break;

	default:
	  usage ();
	}
    }
  if (tmp_string)
    {
      free (argv);
      free (tmp_string);
    }
  return args;
}

/* Grab the next identifier the assembly language source file. This
   state machine is built for speed, not elegance.  */

static struct token *
get_token_asm (FILE *in_FILE, void const *args, int *flags)
{
#define ARGS ((struct args_asm const *) args)
  static int new_line = 1;
  unsigned char const *rct = &ARGS->ctype[1];
  unsigned char *id = scanner_buffer;
  int c;

  obstack_blank (&tokens_obstack, OFFSETOF_TOKEN_NAME);

top:
  c = getc (in_FILE);
  if (ARGS->handle_cpp > 0 && new_line)
    {
      new_line = 0;
      if (c != '#')
	goto next;
      SCAN_CPP_DIRECTIVE;
    }

next:
  while (ISBORING (c))
    c = getc (in_FILE);

  if (ISCOMMENT (c))
    {
      while (ISCBORING (c))
	c = getc (in_FILE);
      new_line = 1;
    }

  if (ISEOF (c))
    {
      new_line = 1;
      obstack_free (&tokens_obstack, obstack_finish (&tokens_obstack));
      return 0;
    }

  if (c == '\n')
    {
      new_line = 1;
      goto top;
    }

  if (c == '/')
    {
      if ((c = getc (in_FILE)) != '*')
	goto next;
      c = getc (in_FILE);
      for (;;)
	{
	  while (ISCCBORING (c))
	    c = getc (in_FILE);
	  c = getc (in_FILE);
	  if (c == '/')
	    {
	      c = getc (in_FILE);
	      break;
	    }
	  else if (ISEOF (c))
	    {
	      new_line = 1;
	      obstack_free (&tokens_obstack, obstack_finish (&tokens_obstack));
	      return 0;
	    }
	}
      goto next;
    }

  id = scanner_buffer;
  if (ARGS->strip_underscore && c == '_' && !ISID1ST (c = getc (in_FILE)))
    {
      obstack_grow0 (&tokens_obstack, "_", 1);
      return (struct token *) obstack_finish (&tokens_obstack);
    }
  *id++ = c;
  if (ISID1ST (c))
    {
      *flags = TOK_NAME;
      while (ISIDREST (c = getc (in_FILE)))
	*id++ = c;
    }
  else if (ISNUMBER (c))
    {
      *flags = TOK_NUMBER;
      while (ISNUMBER (c = getc (in_FILE)))
	*id++ = c;
    }
  else
    {
      if (isprint (c))
	fprintf (stderr, _("junk: `%c'"), c);
      else
	fprintf (stderr, _("junk: `\\%03o'"), c);
      goto next;
    }

  *id = '\0';
  for (id = scanner_buffer; *id; id++)
    if (ISIGNORE (*id))
      goto next;
  ungetc (c, in_FILE);
  *flags |= TOK_LITERAL;
  obstack_grow0 (&tokens_obstack, scanner_buffer, id - scanner_buffer);
  return (struct token *) obstack_finish (&tokens_obstack);
#undef ARGS
}

#undef I1
#undef NM
#undef NL
#undef CM
#undef IG
#undef C1
#undef C2
#undef EF
#undef ISID1ST
#undef ISIDREST
#undef ISNUMBER
#undef ISEOF
#undef ISCOMMENT
#undef ISBORING
#undef ISCBORING
#undef ISCCBORING
#undef ISIGNORE

/*************** Text *******************************************************/

#define I1	0x01		/* 1st char of an identifier [a-zA-Z_] */
#define NM	0x02		/* digit [0-9a-fA-FxX] */
#define SQ	0x04		/* squeeze these out (.,',-) */
#define EF	0x80		/* EOF */

/* Text character classes */
#define ISID1ST(c)	((rct)[c] & (I1))
#define ISIDREST(c)	((rct)[c] & (I1|NM|SQ))
#define ISNUMBER(c)	((rct)[c] & (NM))
#define ISEOF(c)	((rct)[c] & (EF))
#define ISBORING(c)	(!((rct)[c] & (I1|NM|EF)))
#define ISIDSQUEEZE(c)	((rct)[c] & (SQ))

static unsigned char ctype_text[257] =
{
  EF,
/*      0       1       2       3       4       5       6       7   */
/*    -----   -----   -----   -----   -----   -----   -----   ----- */
/*000*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*010*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*020*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*030*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*040*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*050*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*060*/ NM,	NM,	NM,	NM,	NM,	NM,	NM,	NM,
/*070*/ NM,	NM,	0,	0,	0,	0,	0,	0,
/*100*/ 0,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1,
/*110*/ I1,	I1,	I1,	I1,	I1|NM,	I1,	I1,	I1,
/*120*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*130*/ I1|NM,	I1,	I1,	0,	0,	0,	0,	I1,
/*140*/ 0,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1|NM,	I1,
/*150*/ I1,	I1,	I1,	I1,	I1|NM,	I1,	I1,	I1,
/*160*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*170*/ I1|NM,	I1,	I1,	0,	0,	0,	0,	0,
/*200*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*210*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*220*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*230*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*240*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*250*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*260*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*270*/ 0,	0,	0,	0,	0,	0,	0,	0,
/*300*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*310*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*320*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	0,
/*330*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*340*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*350*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
/*360*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	0,
/*370*/ I1,	I1,	I1,	I1,	I1,	I1,	I1,	I1,
};

struct args_text
{
  unsigned char *ctype;
};

static struct args_text args_text = { ctype_text };

static struct option const long_options_text[] =
{
  { "include", required_argument, 0, 'i' },
  { "exclude", required_argument, 0, 'x' },
  { 0 }
};

static void
help_me_text (void)
{
  printf (_("\
Text language:\n\
  -i,--include=CHAR-CLASS  Treat characters of CHAR-CLASS as token constituents\n\
  -x,--exclude=CHAR-CLASS  Treat characters of CHAR-CLASS as token delimiters\n\
"));
}

static void *
parse_args_text (char **argv, int argc)
{
  char *tmp_string = 0;
  struct args_text *args;

  if (argv == 0 || *argv == 0)
    return &args_text;

  if (argc)
    args = &args_text;
  else
    {
      tmp_string = strdup (*argv);
      tokenize_args_string (tmp_string, &argc, &argv);
      args = MALLOC (struct args_text, 1);
      args->ctype = ctype_text;
    }

  optind = 0;
  for (;;)
    {
      int optc = getopt_long (argc, argv, "i:x:",
			      long_options_text, (int *) 0);
      if (optc < 0)
	break;
      if ((optc == 'k' || optc == 'i') && args->ctype == ctype_text)
	args->ctype = CLONE (ctype_text, unsigned char, cardinalityof (ctype_text));
      switch (optc)
	{
	case 'i':
	  set_uchar_ctype (args->ctype, optarg, I1);
	  break;

	case 'x':
	  clear_uchar_ctype (args->ctype, optarg, I1);
	  break;

	default:
	  usage ();
	}
    }
  if (tmp_string)
    {
      free (argv);
      free (tmp_string);
    }
  return args;
}

/* Grab the next identifier the text source file.  This state machine
   is built for speed, not elegance.  */

static struct token *
get_token_text (FILE *in_FILE, void const *args, int *flags)
{
#define ARGS ((struct args_text const *) args)
  unsigned char const *rct = &ARGS->ctype[1];
  int c;
  unsigned char *id = scanner_buffer;

  obstack_blank (&tokens_obstack, OFFSETOF_TOKEN_NAME);

top:
  c = getc (in_FILE);
  while (ISBORING (c))
    c = getc (in_FILE);
  if (ISEOF (c))
    {
      obstack_free (&tokens_obstack, obstack_finish (&tokens_obstack));
      return 0;
    }
  id = scanner_buffer;
  *id++ = c;
  if (ISID1ST (c))
    {
      *flags = TOK_NAME;
      while (ISIDREST (c = getc (in_FILE)))
	if (!ISIDSQUEEZE (c))
	  *id++ = c;
    }
  else if (ISNUMBER (c))
    {
      *flags = TOK_NUMBER;
      while (ISNUMBER (c = getc (in_FILE)))
	*id++ = c;
    }
  else
    {
      if (isprint (c))
	fprintf (stderr, _("junk: `%c'"), c);
      else
	fprintf (stderr, _("junk: `\\%03o'"), c);
      goto top;
    }

  ungetc (c, in_FILE);
  *flags |= TOK_LITERAL;
  obstack_grow0 (&tokens_obstack, scanner_buffer, id - scanner_buffer);
  return (struct token *) obstack_finish (&tokens_obstack);
#undef ARGS
}

#undef I1
#undef NM
#undef SQ
#undef EF
#undef ISID1ST
#undef ISIDREST
#undef ISNUMBER
#undef ISEOF
#undef ISBORING
#undef ISIDSQUEEZE
