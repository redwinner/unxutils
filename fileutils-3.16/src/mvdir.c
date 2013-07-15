/* mvdir -- rename directory on System V r<4
   Copyright (C) 90, 91, 95, 1996 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* Helper program for GNU mv on systems that lack the rename system call.

   Usage: mvdir from to

   FROM must be an existing directory.
   TO must not exist, but its parent must exist.
   This program performs the necessary sanity checking on its arguments.

   Must be installed setuid root.

   Ian Dall (ian@sibyl.eleceng.ua.oz.au)
   and David MacKenzie (djm@gnu.ai.mit.edu) */

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <signal.h>

#include "system.h"
#include "save-cwd.h"
#include "error.h"

//kms
#define link rename

#ifndef HIPRI
#define HIPRI -10
#endif

#ifdef DEBUG
#define link(FROM, TO) (printf("Linking %s to %s\n", FROM, TO), 0)
#define unlink(FILE) (printf("Unlinking %s\n", FILE), 0)
#endif

/* The name this program was run with. */
char *program_name;

char *xgetcwd ();

char *basename ();
char *dirname ();
char *xmalloc ();
void strip_trailing_slashes ();

/* If nonzero, display usage information and exit.  */
static int show_help;

/* If nonzero, print the version on standard output and exit.  */
static int show_version;

static struct option const long_options[] =
{
  {"help", no_argument, &show_help, 1},
  {"version", no_argument, &show_version, 1},
  {0, 0, 0, 0}
};

static void
usage (int status)
{
  if (status != 0)
    fprintf (stderr, _("Try `%s --help' for more information.\n"),
	     program_name);
  else
    {
      printf (_("Usage: %s [OPTION]... EXISTING_DIR NEW_DIR\n"), program_name);
      printf (_("\
Rename EXISTING_DIR to NEW_DIR.\n\
\n\
   --help      display this help and exit\n\
   --version   output version information and exit\n"));
    }
  exit (status);
}

/* Return the full pathname (from /) of the directory DIR,
   in malloc'd storage. */

static char *
fullpath (const char *dir)
{
  struct saved_cwd cwd;
  char *path;

  if (save_cwd (&cwd))
    exit (1);

  path = xgetcwd () ;
  if (path == NULL)
    error (1, errno, _("cannot get current directory"));

  if (restore_cwd (&cwd, _("starting directory"), NULL))
    exit (1);

  free_cwd (&cwd);

  return path+2;
}

int
main (int argc, char **argv)
{
  char *from, *from_parent, *from_base;
  char *to, *to_parent, *to_parent_path, *to_base, *to_dotdot;
  struct stat from_stats, to_stats;
  char *slash;
  int c, i;

  program_name = argv[0];
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  while ((c = getopt_long (argc, argv, "", long_options, (int *) 0)) != EOF)
    {
      switch (c)
	{
	case 0:
	  break;

	default:
	  usage (2);
	}
    }

  if (show_version)
    {
      printf ("mvdir (%s) %s\n", GNU_PACKAGE, VERSION);
      exit (0);
    }

  if (show_help)
    usage (0);

  if (argc - optind != 2)
    usage (2);

  from = argv[optind];
  to = argv[optind + 1];
  strip_trailing_slashes (from);
  strip_trailing_slashes (to);
  from_parent = dirname (from);
  to_parent = dirname (to);
  if (!from_parent || !to_parent)
    error (2, 0, _("virtual memory exhausted"));

  /* Make sure `from' is not "." or "..". */
  from_base = basename (from);
  if (!strcmp (from_base, ".") || !strcmp (from_base, ".."))
    error (1, 0, _("cannot rename `.' or `..'"));

  /* Even with an effective uid of root, link fails if the target exists.
     That is what we want, so don't unlink `to' first.
     However, we do need to check that the directories that link and unlink
     will modify exist and are writable by the user. */

  if (lstat (from, &from_stats))
    error (1, errno, "%s", from);
  if (!S_ISDIR (from_stats.st_mode))
    error (1, 0, _("`%s' is not a directory"), from);
  if (access (from_parent, W_OK))
    error (1, errno, _("cannot write to `%s'"), from_parent);
  if (access (to_parent, W_OK))
    error (1, errno, _("cannot write to `%s'"), to_parent);

  /* To prevent disconnecting the tree rooted at `from' from its parent,
     quit if any of the directories in `to' are the same (dev and ino)
     as the directory `from'. */

  slash = to_parent_path = fullpath (to_parent);
  while (1)
    {
      while (*slash == '\\')
	++slash;
      slash = strchr (slash, '\\');
      if (slash != NULL)
	*slash = '\0';

      if (lstat (to_parent_path, &to_stats))
	error (1, errno, "%s", to_parent_path);
      if (to_stats.st_dev == from_stats.st_dev
	  && to_stats.st_ino == from_stats.st_ino)
	error (1, 0, _("`%s' is an ancestor of `%s'"), from, to);
      if (slash != NULL)
	*slash++ = '\\';
      else
	break;
    }

  /* We can't make the renaming atomic, but we do our best. */
//  for (i = NSIG; i > 0; i--)
//    if (i != SIGKILL)
//      signal (i, SIG_IGN);
  setuid (0);			/* Make real uid 0 so it is harder to kill. */
//  nice (HIPRI - nice (0));	/* Raise priority. */

  if (link (from, to))
    error (1, errno, _("cannot link `%s' to `%s'"), from, to);
  exit (0); //kms
  if (unlink (from))
    error (1, errno, _("cannot unlink `%s'"), from);

  /* Replace the directory's `..' entry.  It used to be a link to
     the parent of `from'; make it a link to the parent of `to' instead.
     To handle the case where `from' is the current directory
     and `to' starts with `../', we go to the full path of `to's parent,
     lest we try to reference the new directory's `..' while creating it.  */

  to_base = basename (to);
  i = strlen (to_base);
  to_dotdot = xmalloc (i + 4);
  strcpy (to_dotdot, to_base);
  strcpy (to_dotdot + i, "\\..");

  if (chdir (to_parent_path))
    error (1, errno, "%s", to_parent_path);
  if (unlink (to_dotdot) && errno != ENOENT)
    error (1, errno, _("cannot unlink `%s'"), to_dotdot);
  if (link (".", to_dotdot))
    error (1, errno, _("cannot link `%s' to `%s'"), ".", to_dotdot);

  exit (0);
}
