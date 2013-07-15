/* idx.c -- simple interface for testing scanners scanners
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
#include <getopt.h>
#include "xstring.h"
#include "xnls.h"
#include "scanners.h"
#include "idfile.h"
#include "pathmax.h"
#include "error.h"

void scan_files __P((struct idhead *idhp));
void scan_member_file __P((struct member_file const *member));
void usage __P((void));

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
  { "include", required_argument, 0, 'i' },
  { "exclude", required_argument, 0, 'x' },
  { "lang-option", required_argument, 0, 'l' },
  { "lang-map", required_argument, 0, 'm' },
  { "default-lang", required_argument, 0, 'd' },
  { "prune", required_argument, 0, 'p' },
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
Print all tokens found in a source file.\n\
  -i, --include=LANGS     include languages in LANGS (default: \"C C++ asm\")\n\
  -x, --exclude=LANGS     exclude languages in LANGS\n\
  -l, --lang-option=L:OPT pass OPT as a default for language L (see below)\n\
  -m, --lang-map=MAPFILE  use MAPFILE to map file names onto source language\n\
  -d, --default-lang=LANG make LANG the default source language\n\
  -p, --prune=NAMES       exclude the named files and/or directories\n\
      --help              display this help and exit\n\
      --version           output version information and exit\n\
\n\
The following arguments apply to the language-specific scanners:\n\
"));
  language_help_me ();
  exit (0);
}

int
main (int argc, char **argv)
{
  program_name = argv[0];

  /* Set locale according to user's wishes.  */
  setlocale (LC_ALL, "");

  /* Tell program which translations to use and where to find.  */
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  for (;;)
    {
      int optc = getopt_long (argc, argv, "i:x:l:m:d:p:",
			      long_options, (int *) 0);
      if (optc < 0)
	break;
      switch (optc)
	{
	case 0:
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
  if (argc == 0)
    {
      static char dot[] = ".";
      static char *dotp = dot;
      argc = 1;
      argv = &dotp;
    }

  language_getopt ();
  if (cw_dlink == 0)
    cw_dlink = init_walker (&idh);
  parse_language_map (lang_map_file_name);

  while (argc--)
    {
      struct file_link *flink = parse_file_name (*argv++, cw_dlink);
      if (flink)
	walk_flink (flink, 0);
    }
  mark_member_file_links (&idh);
  obstack_init (&tokens_obstack);
  scan_files (&idh);

  return 0;
}

void
scan_files (struct idhead *idhp)
{
  struct member_file **members_0
    = (struct member_file **) hash_dump (&idhp->idh_member_file_table,
					 0, member_file_qsort_compare);
  struct member_file **end = &members_0[idhp->idh_member_file_table.ht_fill];
  struct member_file **members;

  if (largest_member_file > MAX_LARGEST_MEMBER_FILE)
    largest_member_file = MAX_LARGEST_MEMBER_FILE;
  scanner_buffer = MALLOC (unsigned char, largest_member_file + 1);

  for (members = members_0; members < end; members++)
    scan_member_file (*members);

  free (scanner_buffer);
  free (members_0);
}

void
scan_member_file (struct member_file const *member)
{
  struct lang_args const *lang_args = member->mf_lang_args;
  struct language const *lang = lang_args->la_language;
  get_token_func_t get_token = lang->lg_get_token;
  struct file_link *flink = member->mf_link;
  FILE *source_FILE;

  chdir_to_link (flink->fl_parent);
  source_FILE = fopen (flink->fl_name, "r");
  if (source_FILE)
    {
      void const *args = lang_args->la_args_digested;
      int flags;
      struct token *token;

      while ((token = (*get_token) (source_FILE, args, &flags)) != NULL)
	{
	  puts (TOKEN_NAME (token));
	  obstack_free (&tokens_obstack, token);
	}
      fclose (source_FILE);
    }
  else
    error (0, errno, _("can't open `%s'"), flink->fl_name);
}
