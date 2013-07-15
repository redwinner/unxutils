/* mkid.c -- build an identifer database
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
#include "xstdlib.h"
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <getopt.h>
#include "xsysstat.h"
#include "xstddef.h"
#include "xunistd.h"
#include "xnls.h"
#include "pathmax.h"
#include "xstring.h"
#include "idfile.h"
#include "xmalloc.h"
#include "hash.h"
#include "scanners.h"
#include "error.h"
#include "xalloca.h"
#if HAVE_LIMITS_H
# include <limits.h>
#endif

struct summary
{
  struct token **sum_tokens;
  unsigned char const *sum_hits;
  struct summary *sum_parent;
  union {
    struct summary *u_kids[8];	/* when sum_level > 0 */
#define sum_kids sum_u.u_kids
    struct member_file *u_files[8];	/* when sum_level == 0 */
#define sum_files sum_u.u_files
  } sum_u;
  unsigned long sum_tokens_size;
  unsigned long sum_hits_count;
  int sum_free_index;
  int sum_level;
};

void usage __P((void));
static void help_me __P((void));
int main __P((int argc, char **argv));
int ceil_log_8 __P((unsigned long n));
int ceil_log_2 __P((unsigned long n));
void assert_writeable __P((char const *file_name));
void scan_files __P((struct idhead *idhp));
void scan_member_file __P((struct member_file const *member));
void scan_member_file_1 __P((get_token_func_t get_token, void const *args, FILE *source_FILE));
void report_statistics __P((void));
void write_id_file __P((struct idhead *idhp));
unsigned long token_hash_1 __P((void const *key));
unsigned long token_hash_2 __P((void const *key));
int token_hash_cmp __P((void const *x, void const *y));
int token_qsort_cmp __P((void const *x, void const *y));
void bump_current_hits_signature __P((void));
void init_hits_signature __P((int i));
void free_summary_tokens __P((void));
void summarize __P((void));
void init_summary __P((void));
struct summary *make_sibling_summary __P((struct summary *summary));
int count_vec_size __P((struct summary *summary, unsigned char const *tail_hits));
int count_buf_size __P((struct summary *summary, unsigned char const *tail_hits));
void assert_hits __P((struct summary* summary));
void write_hits __P((FILE *fp, struct summary *summary, unsigned char const *tail_hits));
void sign_token __P((struct token *token));
void add_token_to_summary __P((struct summary *summary, struct token *token));

struct hash_table token_table;

/* Miscellaneous statistics */
unsigned long input_chars;
unsigned long name_tokens;
unsigned long number_tokens;
unsigned long string_tokens;
unsigned long literal_tokens;
unsigned long comment_tokens;
unsigned long occurrences;
unsigned long hits_length = 0;
unsigned long tokens_length = 0;
unsigned long output_length = 0;

int verbose_flag = 0;
int statistics_flag = 0;

int file_name_count = 0;	/* # of files in database */
int levels = 0;			/* ceil(log(8)) of file_name_count */

char *current_hits_signature;
#define INIT_TOKENS_SIZE(level) (1 << ((level) + 13))
struct summary *summary_root;
struct summary *summary_leaf;

char const *program_name;

char *lang_map_file_name = 0;
int show_version = 0;
int show_help = 0;
struct idhead idh;
struct file_link *cw_dlink;

void
usage (void)
{
  fprintf (stderr, _("Try `%s --help' for more information.\n"),
	   program_name);
  exit (1);
}

static struct option const long_options[] =
{
  { "file", required_argument, 0, 'f' },
  { "output", required_argument, 0, 'o' },
  { "include", required_argument, 0, 'i' },
  { "exclude", required_argument, 0, 'x' },
  { "lang-option", required_argument, 0, 'l' },
  { "lang-map", required_argument, 0, 'm' },
  { "default-lang", required_argument, 0, 'd' },
  { "prune", required_argument, 0, 'p' },
  { "verbose", no_argument, 0, 'v' },
  { "statistics", no_argument, 0, 's' },
  { "help", no_argument, &show_help, 1 },
  { "version", no_argument, &show_version, 1 },
  { 0 }
};

static void
help_me (void)
{
  printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"), program_name);

  printf (_("\
Build an identifier database.\n\
  -o, --output=OUTFILE    file name of ID database output\n\
  -f, --file=OUTFILE      synonym for --output\n\
  -i, --include=LANGS     include languages in LANGS (default: \"C C++ asm\")\n\
  -x, --exclude=LANGS     exclude languages in LANGS\n\
  -l, --lang-option=L:OPT pass OPT as a default for language L (see below)\n\
  -m, --lang-map=MAPFILE  use MAPFILE to map file names onto source language\n\
  -d, --default-lang=LANG make LANG the default source language\n\
  -p, --prune=NAMES       exclude the named files and/or directories\n\
  -v, --verbose           report per file statistics\n\
  -s, --statistics        report statistics at end of run\n\
\n\
      --help              display this help and exit\n\
      --version           output version information and exit\n\
\n\
FILE may be a file name, or a directory name to recursively search.\n\
If no FILE is given, the current directory is searched by default.\n\
Note that the `--include' and `--exclude' options are mutually-exclusive.\n\
\n\
The following arguments apply to the language-specific scanners:\n\
"));
  language_help_me ();
  exit (0);
}

char const *heap_initial;
char const *heap_after_walk;
char const *heap_after_scan;

int
main (int argc, char **argv)
{
  program_name = argv[0];
  heap_initial = (char const *) sbrk (0);
  idh.idh_file_name = DEFAULT_ID_FILE_NAME;

  /* Set locale according to user's wishes.  */
  setlocale (LC_ALL, "");

  /* Tell program which translations to use and where to find.  */
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  for (;;)
    {
      int optc = getopt_long (argc, argv, "o:f:i:x:l:m:d:p:vVs",
			      long_options, (int *) 0);
      if (optc < 0)
	break;
      switch (optc)
	{
	case 0:
	  break;

	case 'o':
	case 'f':
	  idh.idh_file_name = optarg;
	  break;

	case 'i':
	  include_languages (optarg);
	  break;

	case 'x':
	  exclude_languages (optarg);
	  break;

	case 'l':
	  language_save_arg (optarg);
	  break;

	case 'm':
	  lang_map_file_name = optarg;
	  break;

	case 'd':
	  set_default_language (optarg);
	  break;

	case 'p':
	  if (cw_dlink == 0)
	    cw_dlink = init_walker (&idh);
	  prune_file_names (optarg, cw_dlink);
	  break;

	case 'V':
	  walker_verbose_flag = 1;
	case 'v':
	  verbose_flag = 1;
	case 's':
	  statistics_flag = 1;
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

  argc -= optind;
  argv += optind;

  /* If no file or directory options exist, walk the current directory.  */
  if (argc == 0)
    {
      static char dot[] = ".";
      static char *dotp = dot;
      argc = 1;
      argv = &dotp;
    }

  language_getopt ();
  assert_writeable (idh.idh_file_name);
  if (cw_dlink == 0)
    cw_dlink = init_walker (&idh);
  parse_language_map (lang_map_file_name);

  /* Walk the file and directory names given on the command line.  */
  while (argc--)
    {
      struct file_link *flink = parse_file_name (*argv++, cw_dlink);
      if (flink)
	walk_flink (flink, 0);
    }
  heap_after_walk = (char const *) sbrk (0);

  mark_member_file_links (&idh);
  log_8_member_files = ceil_log_8 (idh.idh_member_file_table.ht_fill);
  current_hits_signature = MALLOC (char, log_8_member_files);

  /* If scannable files were given, then scan them.  */
  if (idh.idh_member_file_table.ht_fill)
    {
      scan_files (&idh);
      heap_after_scan = sbrk (0);

      free_summary_tokens ();
      free (token_table.ht_vec);
      chdir_to_link (cw_dlink);
      write_id_file (&idh);

      if (statistics_flag)
	report_statistics ();
    }
  else
    error (0, 0, "nothing to do");

  exit (0);
}

/* Return the integer ceiling of the base-8 logarithm of N.  */

int
ceil_log_8 (unsigned long n)
{
  int log_8 = 0;

  n--;
  while (n)
    {
      log_8++;
      n >>= 3;
    }
  return log_8;
}

/* Return the integer ceiling of the base-8 logarithm of N.  */

int
ceil_log_2 (unsigned long n)
{
  int log_2 = 0;

  n--;
  while (n)
    {
      log_2++;
      n >>= 1;
    }
  return log_2;
}

/* Ensure that FILE_NAME can be written.  If it exists, make sure it's
   writable.  If it doesn't exist, make sure it can be created.  Exit
   with a diagnostic if this is not so.  */

void
assert_writeable (char const *file_name)
{
  if (access (file_name, 06) < 0)
    {
      if (errno == ENOENT)
	{
	  char const *dir_name = dirname (file_name);
	  if (!dir_name || !*dir_name)
	    dir_name = ".";
	  if (access (dir_name, 06) < 0)
	    error (1, errno, _("can't create `%s' in `%s'"),
		   basename (file_name), dir_name);
	}
      else
	error (1, errno, _("can't modify `%s'"), file_name);
    }
}

/* Iterate over all eligible files (the members of the set of scannable files).
   Create a tree8 to store the set of files where a token occurs.  */

void
scan_files (struct idhead *idhp)
{
  struct member_file **members_0
    = (struct member_file **) hash_dump (&idhp->idh_member_file_table,
					 0, member_file_qsort_compare);
  struct member_file **end = &members_0[idhp->idh_member_file_table.ht_fill];
  struct member_file **members = members_0;
  int n = idhp->idh_member_file_table.ht_fill;

  n = n * ceil_log_2 (n) * 16;
  if (n == 0)
    n = 1024;
  else if (n > 1024*1024)
    n = 1024*1024;

  hash_init (&token_table, n, token_hash_1, token_hash_2, token_hash_cmp);
  if (verbose_flag)
    printf ("files=%ld, largest=%lu, slots=%ld\n",
	    idhp->idh_member_file_table.ht_fill,
	    largest_member_file, token_table.ht_size);
  init_hits_signature (0);
  init_summary ();
  obstack_init (&tokens_obstack);

  if (largest_member_file > MAX_LARGEST_MEMBER_FILE)
    largest_member_file = MAX_LARGEST_MEMBER_FILE;
  scanner_buffer = MALLOC (unsigned char, largest_member_file + 1);

  for (;;)
    {
      struct member_file *member = *members++;
      scan_member_file (member);
      if (members == end)
	break;
      if (current_hits_signature[0] & 0x80)
	summarize ();
      bump_current_hits_signature ();
    }

  free (scanner_buffer);
  free (members_0);
}

/* Open a file and scan it.  */

void
scan_member_file (struct member_file const *member)
{
  struct lang_args const *lang_args = member->mf_lang_args;
  struct language const *lang = lang_args->la_language;
  get_token_func_t get_token = lang->lg_get_token;
  struct file_link *flink = member->mf_link;
  struct stat st;
  FILE *source_FILE;

  chdir_to_link (flink->fl_parent);
  source_FILE = fopen (flink->fl_name, "r");
  if (source_FILE)
    {
      char *file_name = ALLOCA (char, PATH_MAX);
      if (statistics_flag)
	{
	  if (fstat (fileno (source_FILE), &st) < 0)
	    {
	      maybe_relative_file_name (file_name, flink, cw_dlink);
	      error (0, errno, _("can't stat `%s'"), file_name);
	    }
	  else
	    input_chars += st.st_size;
	}
      if (verbose_flag)
	{
	  maybe_relative_file_name (file_name, flink, cw_dlink);
	  printf ("%ld: %s: %s", member->mf_index, lang->lg_name, file_name);
	  fflush (stdout);
	}
      scan_member_file_1 (get_token, lang_args->la_args_digested, source_FILE);
      if (verbose_flag)
	putchar ('\n');
      fclose (source_FILE);
    }
  else
    error (0, errno, _("can't open `%s'"), flink->fl_name);
}

/* Iterate over all tokens in the file, and merge the file's tree8
   signature into the token table entry.  */

void
scan_member_file_1 (get_token_func_t get_token, void const *args, FILE *source_FILE)
{
  struct token **slot;
  struct token *token;
  int flags;
  int new_tokens = 0;
  int distinct_tokens = 0;

  while ((token = (*get_token) (source_FILE, args, &flags)) != NULL)
    {
      if (*TOKEN_NAME (token) == '\0') {
	obstack_free (&tokens_obstack, token);
	continue;
      }
      slot = (struct token **) hash_find_slot (&token_table, token);
      if (HASH_VACANT (*slot))
	{
	  token->tok_flags = flags;
	  token->tok_count = 1;
	  memset (TOKEN_HITS (token), 0, log_8_member_files);
	  sign_token (token);
	  if (verbose_flag)
	    {
	      distinct_tokens++;
	      new_tokens++;
	    }
	  hash_insert_at (&token_table, token, slot);
	}
      else
	{
	  obstack_free (&tokens_obstack, token);
	  token = *slot;
	  token->tok_flags |= flags;
	  if (token->tok_count < USHRT_MAX)
	    token->tok_count++;
	  if (!(TOKEN_HITS (token)[0] & current_hits_signature[0]))
	    {
	      sign_token (token);
	      if (verbose_flag)
		distinct_tokens++;
	    }
	}
    }
  if (verbose_flag)
    {
      printf (_("  new = %d/%d"), new_tokens, distinct_tokens);
      if (distinct_tokens != 0)
	printf (" = %.0f%%", 100.0 * (double) new_tokens / (double) distinct_tokens);
    }
}

void
report_statistics (void)
{
  printf (_("Name=%ld, "), name_tokens);
  printf (_("Number=%ld, "), number_tokens);
  printf (_("String=%ld, "), string_tokens);
  printf (_("Literal=%ld, "), literal_tokens);
  printf (_("Comment=%ld\n"), comment_tokens);

  printf (_("Files=%ld, "), idh.idh_files);
  printf (_("Tokens=%ld, "), occurrences);
  printf (_("Bytes=%ld Kb, "), input_chars / 1024);
  printf (_("Heap=%d+%d Kb, "), (heap_after_scan - heap_after_walk) / 1024,
	  (heap_after_walk - heap_initial) / 1024);
  printf (_("Output=%ld (%ld tok, %ld hit)\n"), output_length, tokens_length, hits_length);

  hash_print_stats (&token_table, stdout);
  printf (_(", Freq=%ld/%ld=%.2f\n"), occurrences, token_table.ht_fill,
	  (double) occurrences / (double) token_table.ht_fill);
}

/* As the database is written, may need to adjust the file names.  If
   we are generating the ID file in a remote directory, then adjust
   the file names to be relative to the location of the ID database.

   (This would be a common useage if you want to make a database for a
   directory which you have no write access to, so you cannot create
   the ID file.)  */

void
write_id_file (struct idhead *idhp)
{
  struct token **tokens;
  int i;
  int buf_size;
  int vec_size;
  int tok_size;
  int max_buf_size = 0;
  int max_vec_size = 0;

  if (verbose_flag)
    printf (_("Sorting tokens...\n"));
  assert (summary_root->sum_hits_count == token_table.ht_fill);
  tokens = REALLOC (summary_root->sum_tokens, struct token *, token_table.ht_fill);
  qsort (tokens, token_table.ht_fill, sizeof (struct token *), token_qsort_cmp);

  if (verbose_flag)
    printf (_("Writing `%s'...\n"), idhp->idh_file_name);
  idhp->idh_FILE = fopen (idhp->idh_file_name, "w+b");
  if (idhp->idh_FILE == NULL)
    error (1, errno, _("can't create `%s'"), idhp->idh_file_name);

  idhp->idh_magic[0] = IDH_MAGIC_0;
  idhp->idh_magic[1] = IDH_MAGIC_1;
  idhp->idh_version = IDH_VERSION;
  idhp->idh_flags = IDH_COUNTS;

  /* write out the list of pathnames */

  fseek (idhp->idh_FILE, sizeof_idhead (), 0);
  idhp->idh_flinks_offset = ftell (idhp->idh_FILE);
  serialize_file_links (idhp);

  /* write out the list of identifiers */

  putc ('\0', idhp->idh_FILE);
  putc ('\0', idhp->idh_FILE);
  idhp->idh_tokens_offset = ftell (idhp->idh_FILE);

  for (i = 0; i < token_table.ht_fill; i++, tokens++)
    {
      struct token *token = *tokens;
      occurrences += token->tok_count;
      if (token->tok_flags & TOK_NUMBER)
	number_tokens++;
      if (token->tok_flags & TOK_NAME)
	name_tokens++;
      if (token->tok_flags & TOK_STRING)
	string_tokens++;
      if (token->tok_flags & TOK_LITERAL)
	literal_tokens++;
      if (token->tok_flags & TOK_COMMENT)
	comment_tokens++;

      fputs (TOKEN_NAME (token), idhp->idh_FILE);
      putc ('\0', idhp->idh_FILE);
      if (token->tok_count > 0xff)
	token->tok_flags |= TOK_SHORT_COUNT;
      putc (token->tok_flags, idhp->idh_FILE);
      putc (token->tok_count & 0xff, idhp->idh_FILE);
      if (token->tok_flags & TOK_SHORT_COUNT)
	putc (token->tok_count >> 8, idhp->idh_FILE);

      vec_size = count_vec_size (summary_root, TOKEN_HITS (token) + levels);
      buf_size = count_buf_size (summary_root, TOKEN_HITS (token) + levels);
      hits_length += buf_size;
      tok_size = strlen (TOKEN_NAME (token)) + 1;
      tokens_length += tok_size;
      buf_size += tok_size + sizeof (token->tok_flags) + sizeof (token->tok_count) + 2;
      if (buf_size > max_buf_size)
	max_buf_size = buf_size;
      if (vec_size > max_vec_size)
	max_vec_size = vec_size;

      write_hits (idhp->idh_FILE, summary_root, TOKEN_HITS (token) + levels);
      putc ('\0', idhp->idh_FILE);
      putc ('\0', idhp->idh_FILE);
    }
  assert_hits (summary_root);
  idhp->idh_tokens = token_table.ht_fill;
  output_length = ftell (idhp->idh_FILE);
  idhp->idh_end_offset = output_length - 2;
  idhp->idh_buf_size = max_buf_size;
  idhp->idh_vec_size = max_vec_size;

  write_idhead (&idh);
  fclose (idhp->idh_FILE);
}

/* Define primary and secondary hash and comparison functions for the
   token table.  */

unsigned long
token_hash_1 (void const *key)
{
  return_STRING_HASH_1 (TOKEN_NAME ((struct token const *) key));
}

unsigned long
token_hash_2 (void const *key)
{
  return_STRING_HASH_2 (TOKEN_NAME ((struct token const *) key));
}

int
token_hash_cmp (void const *x, void const *y)
{
  return_STRING_COMPARE (TOKEN_NAME ((struct token const *) x),
			 TOKEN_NAME ((struct token const *) y));
}

int
token_qsort_cmp (void const *x, void const *y)
{
  return_STRING_COMPARE (TOKEN_NAME (*(struct token const *const *) x),
			 TOKEN_NAME (*(struct token const *const *) y));
}


/****************************************************************************/

/* Advance the tree8 hit signature that corresponds to the file
   we are now scanning.  */

void
bump_current_hits_signature (void)
{
  unsigned char *hits = current_hits_signature;
  while (*hits & 0x80)
    *hits++ = 1;
  *hits <<= 1;
}

/* Initialize the tree8 hit signature for the first file we scan.  */

void
init_hits_signature (int i)
{
  unsigned char *hits = current_hits_signature;
  unsigned char const *end = current_hits_signature + log_8_member_files;
  while (hits < end)
    {
      *hits = 1 << (i & 7);
      i >>= 3;
      hits++;
    }
}

void
free_summary_tokens (void)
{
  struct summary *summary = summary_leaf;
  while (summary != summary_root)
    {
      free (summary->sum_tokens);
      summary = summary->sum_parent;
    }
}

void
summarize (void)
{
  unsigned char const *hits_sig = current_hits_signature;
  struct summary *summary = summary_leaf;

  do
    {
      unsigned long count = summary->sum_hits_count;
      unsigned char *hits = MALLOC (unsigned char, count + 1);
      unsigned int level = summary->sum_level;
      struct token **tokens = summary->sum_tokens;
      unsigned long init_size = INIT_TOKENS_SIZE (summary->sum_level);

      if (verbose_flag)
	printf (_("level %d: %ld/%ld = %.0f%%\n"),
		summary->sum_level, count, init_size,
		100.0 * (double) count / (double) init_size);

      qsort (tokens, count, sizeof (struct token *), token_qsort_cmp);
      summary->sum_hits = hits;
      while (count--)
	{
	  unsigned char *hit = TOKEN_HITS (*tokens++) + level;
	  *hits++ = *hit;
	  *hit = 0;
	}
      *hits++ = 0;
      if (summary->sum_parent)
	{
	  free (summary->sum_tokens);
	  summary->sum_tokens = 0;
	}
      summary = summary->sum_parent;
    }
  while (*++hits_sig & 0x80);
  summary_leaf = make_sibling_summary (summary_leaf);
}

void
init_summary (void)
{
  unsigned long size = INIT_TOKENS_SIZE (0);
  summary_root = summary_leaf = CALLOC (struct summary, 1);
  summary_root->sum_tokens_size = size;
  summary_root->sum_tokens = MALLOC (struct token *, size);
}

struct summary *
make_sibling_summary (struct summary *summary)
{
  struct summary *parent = summary->sum_parent;
  unsigned long size;

  if (parent == NULL)
    {
      levels++;
      summary_root = summary->sum_parent = parent = CALLOC (struct summary, 1);
      parent->sum_level = levels;
      parent->sum_kids[0] = summary;
      parent->sum_hits_count = summary->sum_hits_count;
      parent->sum_free_index = 1;
      size = INIT_TOKENS_SIZE (levels);
      if (summary->sum_tokens_size >= size)
	{
	  parent->sum_tokens_size = summary->sum_tokens_size;
	  parent->sum_tokens = summary->sum_tokens;
	}
      else
	{
	  parent->sum_tokens_size = size;
	  parent->sum_tokens = REALLOC (summary->sum_tokens, struct token *, size);
	}
      summary->sum_tokens = 0;
    }
  if (parent->sum_free_index == 8)
    parent = make_sibling_summary (parent);
  summary = CALLOC (struct summary, 1);
  summary->sum_level = parent->sum_level - 1;
  parent->sum_kids[parent->sum_free_index++] = summary;
  summary->sum_parent = parent;
  size = INIT_TOKENS_SIZE (summary->sum_level);
  summary->sum_tokens_size = size;
  summary->sum_tokens = MALLOC (struct token *, size);
  return summary;
}

int
count_vec_size (struct summary *summary, unsigned char const *tail_hits)
{
  struct summary **kids;
  unsigned int hits = (summary->sum_hits ? *summary->sum_hits : *tail_hits);

  kids = summary->sum_kids;
  if (*kids == NULL)
    {
      static char bits_per_nybble[] = { 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4 };
      return bits_per_nybble[hits & 0xf] + bits_per_nybble[hits >> 4];
    }
  else
    {
      int bit;
      int count = 0;
      --tail_hits;
      for (bit = 1; bit & 0xff; bit <<= 1, ++kids)
	if (bit & hits)
	  count += count_vec_size (*kids, tail_hits);
      return count;
    }
}

int
count_buf_size (struct summary *summary, unsigned char const *tail_hits)
{
  struct summary **kids;
  unsigned int hits = (summary->sum_hits ? *summary->sum_hits : *tail_hits);

  kids = summary->sum_kids;
  if (*kids == NULL)
    return 1;
  else
    {
      int bit;
      int count = 1;
      --tail_hits;
      for (bit = 1; bit & 0xff; bit <<= 1, ++kids)
	if (bit & hits)
	  count += count_buf_size (*kids, tail_hits);
      return count;
    }
}

void
assert_hits (struct summary* summary)
{
  struct summary **kids = summary->sum_kids;
  struct summary **end = &kids[8];

  assert (summary->sum_hits == NULL || *summary->sum_hits == 0);

  if (end[-1] == 0)
    while (*--end == 0)
      ;
  while (kids < end)
    assert_hits (*kids++);
}

void
write_hits (FILE *fp, struct summary *summary, unsigned char const *tail_hits)
{
  struct summary **kids;
  unsigned int hits = (summary->sum_hits ? *summary->sum_hits++ : *tail_hits);

  assert (hits);
  putc (hits, fp);

  kids = summary->sum_kids;
  if (*kids)
    {
      int bit;
      --tail_hits;
      for (bit = 1; (bit & 0xff) && *kids; bit <<= 1, ++kids)
	if (bit & hits)
	  write_hits (fp, *kids, tail_hits);
    }
}

void
sign_token (struct token *token)
{
  unsigned char *tok_hits = TOKEN_HITS (token);
  unsigned char *hits_sig = current_hits_signature;
  unsigned char *end = current_hits_signature + log_8_member_files;
  struct summary *summary = summary_leaf;

  while (summary)
    {
      if (*tok_hits == 0)
	add_token_to_summary (summary, token);
      if (*tok_hits & *hits_sig)
	break;
      *tok_hits |= *hits_sig;
      summary = summary->sum_parent;
      tok_hits++;
      hits_sig++;
    }
  while (hits_sig < end)
    {
      if (*tok_hits & *hits_sig)
	break;
      *tok_hits |= *hits_sig;
      tok_hits++;
      hits_sig++;
    }
}

void
add_token_to_summary (struct summary *summary, struct token *token)
{
  unsigned long size = summary->sum_tokens_size;

  if (summary->sum_hits_count >= size)
    {
      size *= 2;
      summary->sum_tokens = REALLOC (summary->sum_tokens, struct token *, size);
      summary->sum_tokens_size = size;
    }
  summary->sum_tokens[summary->sum_hits_count++] = token;
}

#ifdef WIN32

#include <malloc.h>

void* sbrk(int action)
{
//  return (void*) GetProcessHeap();
return 0;

}
#endif