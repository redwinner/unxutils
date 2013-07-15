/* walker.c -- nifty file-tree walker
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
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <config.h>
#include "xsysstat.h"
#include <stdio.h>
#include "xstdlib.h"
#include "xstddef.h"
#include "xunistd.h"
#include "xstring.h"
#include "xfnmatch.h"
#include "xdirent.h"
#include "xnls.h"
#include "idfile.h"
#include "error.h"
#include "xmalloc.h"
#include "dynvec.h"
#include "scanners.h"
#include "pathmax.h"
#include "xalloca.h"

int walker_verbose_flag = 0;
off_t largest_member_file = 0;

int walk_dir __P((struct file_link *dir_link));
struct member_file *get_member_file __P((struct file_link *flink));
struct lang_args *get_lang_args __P((struct file_link const *flink));
void print_member_file __P((struct member_file *member));
int walk_sub_dirs __P((struct dynvec *sub_dirs_vec));
void reparent_children __P((struct file_link *dlink, struct file_link *slink));
int classify_link __P((struct file_link *flink, struct stat *stp));
struct file_link *get_link_from_dirent __P((struct dirent *dirent, struct file_link *parent));
struct file_link *make_link_from_dirent __P((struct dirent *dirent, struct file_link *parent));
struct file_link *get_link_from_string __P((char const *name, struct file_link *parent));
struct file_link *make_link_from_string __P((char const *name, struct file_link *parent));
int lang_wanted __P((char const *lang_name));
char **append_strings_to_vector __P((char **vector_0, char *string, char const *delimiter_class));
int vector_length __P((char **vector));
int string_in_vector __P((char const *string, char **vector));
static int same_as_dot __P((char const *cwd));
struct file_link const **fill_link_vector __P((struct file_link const **vec_buf, struct file_link const *flink));
struct file_link const **fill_link_vector_1 __P((struct file_link const **vec_buf, struct file_link const *flink));
char *fill_dot_dots __P((char *buf, int levels));
static char *absolute_file_name_1 __P((char *buffer, struct file_link const *flink));
unsigned long member_file_hash_1 __P((void const *key));
unsigned long member_file_hash_2 __P((void const *key));
int member_file_hash_compare __P((void const *x, void const *y));
unsigned long file_link_hash_1 __P((void const *key));
unsigned long file_link_hash_2 __P((void const *key));
int file_link_hash_compare __P((void const *x, void const *y));
unsigned long dev_ino_hash_1 __P((void const *key));
unsigned long dev_ino_hash_2 __P((void const *key));
int dev_ino_hash_compare __P((void const *x, void const *y));
int symlink_ancestry __P((struct file_link *flink));

#if HAVE_LINK
struct file_link *find_alias_link __P((struct file_link *flink, struct stat *stp));
struct member_file *maybe_get_member_file __P((struct file_link *flink, struct stat *stp));
#endif

#define IS_DOT(s) ((s)[0] == '.' && (s)[1] == '\0')
#define IS_DOT_DOT(s) ((s)[0] == '.' && (s)[1] == '.' && (s)[2] == '\0')
#define IS_DOT_or_DOT_DOT(s) \
    (((s)[0] == '.') && (((s)[1] == '\0') || ((s)[1] == '.' && (s)[2] == '\0')))

static struct file_link *current_dir_link = 0;

static char const white_space[] = " \t\r\n\v\f";

char* xgetcwd __P((void));


/****************************************************************************/
/* Walk the file-system tree rooted at `dir_link', looking for files
   that are eligible for scanning.  */

int
walk_dir (struct file_link *dir_link)
{
  int scannable_files;
  struct dynvec *sub_dirs_vec;
  DIR *dirp;

  if (!chdir_to_link (dir_link))
    return 0;
  dirp = opendir (".");
  if (dirp == 0)
    {
      char *file_name = ALLOCA (char, PATH_MAX);
      absolute_file_name (file_name, dir_link);
      error (0, errno, _("can't read directory `%s' (`.' from `%s')"), file_name, xgetcwd ());
      return 0;
    }
  sub_dirs_vec = make_dynvec (32);
  scannable_files = 0;
  for (;;)
    {
      struct file_link *flink;
      struct dirent *dirent = readdir (dirp);

      if (dirent == 0)
	break;
      if (IS_DOT_or_DOT_DOT (dirent->d_name))
	continue;

      flink = get_link_from_dirent (dirent, dir_link);
      if (!(flink->fl_flags & FL_PRUNE))
	walk_flink (flink, sub_dirs_vec);
    }
  closedir (dirp);

  scannable_files += walk_sub_dirs (sub_dirs_vec);
  dynvec_free (sub_dirs_vec);
  return scannable_files;
}

/* Walk the directories found by walk_dir, calling walk_dir
   recursively for each directory. */

int
walk_sub_dirs (struct dynvec *sub_dirs_vec)
{
  struct file_link **sub_dirs;
  struct file_link **sub_dirs_end;
  int total_scannable_files = 0;

  dynvec_freeze (sub_dirs_vec);
  sub_dirs_end = (struct file_link **)
    &sub_dirs_vec->dv_vec[sub_dirs_vec->dv_fill];
  sub_dirs = (struct file_link **) sub_dirs_vec->dv_vec;
  for ( ; sub_dirs < sub_dirs_end; sub_dirs++)
    {
      struct file_link *sub_dir_link = *sub_dirs;
      int scannable_files = walk_dir (sub_dir_link);
      if (scannable_files)
	total_scannable_files += scannable_files;
    }
  return total_scannable_files;
}

void
walk_flink (struct file_link *flink, struct dynvec *sub_dirs_vec)
{
  struct stat st;
  unsigned int old_flags;
  unsigned int new_flags;

  new_flags = classify_link (flink, &st);
  if (new_flags == 0)
    return;

  old_flags = flink->fl_flags;
  if ((old_flags & FL_TYPE_MASK)
      && (old_flags & FL_TYPE_MASK) != (new_flags & FL_TYPE_MASK))
    {
      char *file_name = ALLOCA (char, PATH_MAX);
      absolute_file_name (file_name, flink);
      error (0, 0, _("notice: `%s' was a %s, but is now a %s!"), file_name,
	     (FL_IS_FILE (old_flags) ? _("file") : _("directory")),
	     (FL_IS_FILE (new_flags) ? _("file") : _("directory")));
    }

  flink->fl_flags = (old_flags & ~(FL_TYPE_MASK|FL_SYM_LINK)) | new_flags;
  if (FL_IS_DIR (new_flags))
    {
#ifndef WIN32
      struct file_link *alias_link = find_alias_link (flink, &st);
#else
struct file_link *alias_link =NULL;
#endif

      if (alias_link)
	{
	  if (!(new_flags & FL_SYM_LINK))
	    reparent_children (flink, alias_link);
	}
      else if (sub_dirs_vec == 0)
	walk_dir (flink);
      else
	dynvec_append (sub_dirs_vec, flink);
    }
  else
    {
      struct member_file *member;
#if HAVE_LINK
      member = maybe_get_member_file (flink, &st);
#else
      member = get_member_file (flink);
#endif
      if (member)
	{
	  if (st.st_size > largest_member_file)
	    largest_member_file = st.st_size;
	  if (walker_verbose_flag)
	    print_member_file (member);
	}
    }
}

/* Take child file_link nodes from a symlinked directory and give them
   to a hard linked directory.  This is something of a pain since a
   file_link's parent node is part of its hash-table key.  We must
   search the entire hash-table for the children.  With each child, we
   must delete it, reset its parent link, then reinsert.  */

void
reparent_children (struct file_link *dlink, struct file_link *slink)
{
  void **slot = idh.idh_file_link_table.ht_vec;
  void **end = &idh.idh_file_link_table.ht_vec[idh.idh_file_link_table.ht_size];

  for ( ; slot < end; slot++)
    {
      if (!HASH_VACANT (*slot))
	{
	  struct file_link *child = (struct file_link *) *slot;
	  if (child->fl_parent == slink)
	    {
	      void **new_slot;
	      *slot = hash_deleted_item;
	      child->fl_parent = dlink;
	      new_slot = hash_find_slot (&idh.idh_file_link_table, child);
	      *new_slot = child;
	    }
	}
    }
}


/****************************************************************************/
/* Separate the wheat from the chaff.  Mark those file_links that are
   components in member files.  */

void
mark_member_file_links (struct idhead *idhp)
{
  struct member_file **members_0
    = (struct member_file **) hash_dump (&idhp->idh_member_file_table,
					 0, member_file_qsort_compare);
  struct member_file **end = &members_0[idhp->idh_member_file_table.ht_fill];
  struct member_file **members;
  long new_index = 0;

  for (members = members_0; members < end; members++)
    {
      struct member_file *member = *members;
      struct file_link *flink;
      member->mf_index = new_index++;
      for (flink = member->mf_link;
	   !(flink->fl_flags & FL_USED); flink = flink->fl_parent)
	flink->fl_flags |= FL_USED;
    }
  free (members_0);
}


#if HAVE_LINK

/****************************************************************************/
/* Return a `member_file' for this `flink' *if* the filename matches
   some scan pattern, and no alias for the file takes precedence ([1]
   hard-links dominate symbolic-links; [2] for two hard-links: first
   come, first served).  */

struct member_file *
maybe_get_member_file (struct file_link *flink, struct stat *stp)
{
  struct file_link *alias_link;
  struct member_file *member;
  struct member_file *alias_member = 0;

  member = get_member_file (flink);
  alias_link = find_alias_link (flink, stp);
  if (alias_link)
    alias_member = find_member_file (alias_link);

  if (member && alias_member)
    {
      int ancestry = symlink_ancestry (flink);
      int alias_ancestry = symlink_ancestry (alias_link);
      if (member->mf_lang_args != alias_member->mf_lang_args)
	{
	  char *file_name = ALLOCA (char, PATH_MAX);
	  char *alias_file_name = ALLOCA (char, PATH_MAX);
	  absolute_file_name (file_name, flink);
	  absolute_file_name (alias_file_name, alias_link);
	  error (0, 0, _("warning: `%s' and `%s' are the same file, but yield different scans!"),
		 file_name, alias_file_name);
	}
      else if (alias_ancestry > ancestry)
	{
	  hash_delete (&idh.idh_member_file_table, member);
	  member->mf_link->fl_flags &= ~FL_MEMBER;
	  return 0;
	}
      else
	{
	  hash_delete (&idh.idh_member_file_table, alias_member);
	  alias_member->mf_link->fl_flags &= ~FL_MEMBER;
	}
    }
  return member;
}

/* Return a previously registered alias for `flink', if any.  */

struct file_link *
find_alias_link (struct file_link *flink, struct stat *stp)
{
  struct dev_ino *dev_ino;
  struct dev_ino **slot;

  dev_ino = (struct dev_ino *) obstack_alloc (&idh.idh_dev_ino_obstack, sizeof (struct dev_ino));
  dev_ino->di_dev = stp->st_dev;
  dev_ino->di_ino = stp->st_ino;
  slot = (struct dev_ino **) hash_find_slot (&idh.idh_dev_ino_table, dev_ino);
  if (HASH_VACANT (*slot))
    {
      dev_ino->di_link = flink;
      hash_insert_at (&idh.idh_dev_ino_table, dev_ino, slot);
      return 0;
    }
  else
    {
      obstack_free (&idh.idh_dev_ino_obstack, dev_ino);
      return (*slot)->di_link;
    }
}

/* Return the distance from `flink' to a symbolic-link ancestor
   directory.  PATH_MAX is considered an infinite distance (e.g.,
   there are no symlinks between `flink' and the root).  */

int
symlink_ancestry (struct file_link *flink)
{
  int ancestry = 0;
  while (!IS_ROOT_FILE_LINK (flink))
    {
      if (flink->fl_flags & FL_SYM_LINK)
	return ancestry;
      ancestry++;
      flink = flink->fl_parent;
    }
  return PATH_MAX;
}

#endif /* HAVE_LINK */

struct member_file *
get_member_file (struct file_link *flink)
{
  struct member_file *member;
  struct member_file **slot;
  struct lang_args const *args;

  args = get_lang_args (flink);
  if (args == 0)
    return 0;
  if (!lang_wanted (args->la_language->lg_name))
    return 0;

  member = (struct member_file *) obstack_alloc (&idh.idh_member_file_obstack,
						 sizeof (struct member_file));
  member->mf_link = flink;
  slot = (struct member_file **) hash_find_slot (&idh.idh_member_file_table, member);
  if (HASH_VACANT (*slot))
    {
      member->mf_index = -1;
      hash_insert_at (&idh.idh_member_file_table, member, slot);
      flink->fl_flags |= FL_MEMBER;
    }
  else
    {
      obstack_free (&idh.idh_member_file_obstack, member);
#if 0
      if (member->mf_lang_args != args)
	{
	  char *file_name = ALLOCA (char, PATH_MAX);
	  absolute_file_name (file_name, flink);
	  error (0, 0, _("notice: scan parameters changed for `%s'"), file_name);
	  member->mf_old_index = -1;
	}
#endif
      member = *slot;
    }
  member->mf_lang_args = args;
  return member;
}

struct member_file *
find_member_file (struct file_link const *flink)
{
  struct member_file key;
  struct member_file **slot;

  key.mf_link = (struct file_link *) flink;
  slot = (struct member_file **) hash_find_slot (&idh.idh_member_file_table, &key);
  if (HASH_VACANT (*slot))
    return 0;
  return *slot;
}

/* March down the list of lang_args, and return the first one whose
   pattern matches FLINK.  Return the matching lang_args, if a
   scanner exists for that language, otherwise return 0.  */

struct lang_args *
get_lang_args (struct file_link const *flink)
{
  struct lang_args *args = lang_args_list;
  char *file_name = ALLOCA (char, PATH_MAX);

  while (args)
    {
      if (strchr (args->la_pattern, SLASH_CHAR))
	{
	  absolute_file_name (file_name, flink);
	  if (fnmatch (args->la_pattern, file_name, MAYBE_FNM_CASEFOLD | FNM_FILE_NAME) == 0)
	    return (args->la_language ? args : 0);
	}
      else
	{
	  if (fnmatch (args->la_pattern, flink->fl_name, MAYBE_FNM_CASEFOLD) == 0)
	    return (args->la_language ? args : 0);
	}
      args = args->la_next;
    }
  return ((lang_args_default && lang_args_default->la_language)
	  ? lang_args_default : 0);
}

void
print_member_file (struct member_file *member)
{
  char *file_name = ALLOCA (char, PATH_MAX);
  absolute_file_name (file_name, member->mf_link);
  printf ("%ld: %s: %s\n", idh.idh_member_file_table.ht_fill - 1,
	  member->mf_lang_args->la_language->lg_name, file_name);
}


/****************************************************************************/

static char **langs_included;
static char **langs_excluded;

int
lang_wanted (char const *lang_name)
{
  if (langs_excluded)
    return !string_in_vector (lang_name, langs_excluded);
  else if (langs_included)
    return string_in_vector (lang_name, langs_included);
  else
    return 1;
}

void
include_languages (char *lang_names)
{
  if (langs_excluded)
    error (1, 0, "can't mix --include and --exclude options");
  langs_included = append_strings_to_vector (langs_included, lang_names, white_space);
}

void
exclude_languages (char *lang_names)
{
  if (langs_excluded)
    error (1, 0, "can't mix --include and --exclude options");
  langs_excluded = append_strings_to_vector (langs_excluded, lang_names, white_space);
}

char **
append_strings_to_vector (char **vector_0, char *string, char const *delimiter_class)
{
  char **vector;
  if (vector_0)
    {
      int length = vector_length (vector_0);
      vector_0 = REALLOC (vector_0, char *,
			  length + 2 + strlen (string) / 2);
      vector = &vector_0[length];
    }
  else
    vector = vector_0 = MALLOC (char *, 2 + strlen (string) / 2);

  *vector = strtok (string, delimiter_class);
  while (*vector)
    *++vector = strtok (0, delimiter_class);
  return REALLOC (vector_0, char *, ++vector - vector_0);
}

int
vector_length (char **vector)
{
  int length = 0;
  while (*vector++)
    length++;
  return length;
}

int
string_in_vector (char const *string, char **vector)
{
  while (*vector)
    if (strequ (string, *vector++))
      return 1;
  return 0;
}


/****************************************************************************/
/* Convert a file name string to an absolute chain of `file_link's.  */

struct file_link *
parse_file_name (char *file_name, struct file_link *relative_dir_link)
{
  struct file_link *flink;
  char **links_0;
  char **links;

  if (IS_ABSOLUTE (file_name))
    {
#if HAVE_LINK
      flink = get_link_from_string (SLASH_STRING, 0);
#else
      flink = 0;
#endif
    }
  else if (relative_dir_link)
    flink = relative_dir_link;
  else if (current_dir_link)
    flink = current_dir_link;
  else
    flink = get_current_dir_link ();

  links = links_0 = vectorize_string (file_name, SLASH_STRING);
  while (*links)
    {
      char const* link_name = *links++;
      if (*link_name == '\0' || IS_DOT (link_name))
	;
      else if (IS_DOT_DOT (link_name))
	flink = flink->fl_parent;
      else
	{
	  struct stat st;
	  flink = get_link_from_string (link_name, flink);
	  if (!flink->fl_flags)
	    {
	      flink->fl_flags = classify_link (flink, &st);
	      if (!flink->fl_flags)
		return 0;
	    }
	}
    }
  free (links_0);
  return flink;
}

/* Return an absolute chain of `file_link's representing the current
   working directory.  */

struct file_link *
get_current_dir_link (void)
{
  struct file_link *dir_link;
  char *cwd_0;
  char *cwd;
  char *xcwd = 0;
  char **links_0;
  char **links;

  if (current_dir_link)
    return current_dir_link;

  cwd_0 = getenv ("PWD");
  if (cwd_0)
    cwd_0 = strdup (cwd_0);
  if (!same_as_dot (cwd_0))
    cwd_0 = xcwd = xgetcwd ();
  if (cwd_0 == 0)
    error (1, errno, _("can't get working directory"));
  cwd = cwd_0;
#if HAVE_LINK
  dir_link = get_link_from_string (SLASH_STRING, 0);
  dir_link->fl_flags = (dir_link->fl_flags & ~FL_TYPE_MASK) | FL_TYPE_DIR;
#else
  dir_link = 0;
#endif
  links = links_0 = vectorize_string (cwd, SLASH_STRING);
  while (*links)
    {
      struct stat st;
      char const* link_name = *links++;
      dir_link = get_link_from_string (link_name, dir_link);
      if (!dir_link->fl_flags)
	dir_link->fl_flags = classify_link (dir_link, &st);
    }
  chdir_to_link (dir_link);
  free (links_0);
  if (xcwd)
    free (xcwd);
  current_dir_link = dir_link;
  return dir_link;
}

static int
same_as_dot (char const *cwd)
{
  struct stat cwd_st;
  struct stat dot_st;

  if (cwd == 0 || *cwd != '/'
      || stat (cwd, &cwd_st) < 0
      || stat (".", &dot_st) < 0)
    return 0;
  return ((cwd_st.st_ino == dot_st.st_ino) && (cwd_st.st_dev == dot_st.st_dev));
}

/* Change the working directory to the directory represented by
   `dir_link'.  Choose the shortest path to the destination based on
   the cached value of the current directory.  */

int
chdir_to_link (struct file_link *dir_link)
{
  char *to_dir_name = ALLOCA (char, PATH_MAX);

  if (current_dir_link == dir_link)
    return 1;

  if (current_dir_link)
    maybe_relative_file_name (to_dir_name, dir_link, current_dir_link);
  else
    absolute_file_name (to_dir_name, dir_link);
  if (chdir (to_dir_name) < 0)
    {
      if (IS_ABSOLUTE (to_dir_name))
	error (0, errno, _("can't chdir to `%s'"), to_dir_name);
      else
	{
	  char *from_dir_name = ALLOCA (char, PATH_MAX);
	  absolute_file_name (from_dir_name, current_dir_link);
	  error (0, errno, _("can't chdir to `%s' from `%s'"), to_dir_name, from_dir_name);
	}
      return 0;
    }
  else
    {
      current_dir_link = dir_link;
      return 1;
    }
}

char **
vectorize_string (char *string, char const *delimiter_class)
{
  char **vector_0 = MALLOC (char *, 2 + strlen (string) / 2);
  char **vector = vector_0;

  *vector = strtok (string, delimiter_class);
  while (*vector)
    *++vector = strtok (0, delimiter_class);
  return REALLOC (vector_0, char *, ++vector - vector_0);
}

void
prune_file_names (char *str, struct file_link *from_link)
{
  char **file_names_0 = vectorize_string (str, white_space);
  char **file_names = file_names_0;

  while (*file_names)
    {
      struct file_link *flink = parse_file_name (*file_names++, from_link);
      if (flink)
	flink->fl_flags |= FL_PRUNE;
    }
  free (file_names_0);
}


/****************************************************************************/
/* Gather information about the link at `flink'.  If it's a good
   file or directory, return its mod-time and type.  */

int
classify_link (struct file_link *flink, struct stat *stp)
{
  unsigned int flags = 0;

  if (!chdir_to_link (flink->fl_parent))
    return 0;

#ifdef S_IFLNK
  if (lstat (flink->fl_name, stp) < 0)
    {
      error (0, errno, _("can't lstat `%s' from `%s'"), flink->fl_name, xgetcwd ());
      return 0;
    }
  if (S_ISLNK (stp->st_mode))
    {
#endif
      if (stat (flink->fl_name, stp) < 0)
	{
	  error (0, errno, _("can't stat `%s' from `%s'"), flink->fl_name, xgetcwd ());
	  return 0;
	}
#ifdef S_IFLNK
      flags |= FL_SYM_LINK;
    }
#endif
  if (stp->st_size == 0)
    return 0;
  else if (S_ISDIR (stp->st_mode))
    flags |= FL_TYPE_DIR;
  else if (S_ISREG (stp->st_mode))
    flags |= FL_TYPE_FILE;
  else
    return 0;
  return flags;
}


/****************************************************************************/
/* Retrieve an existing flink; or if none exists, create one. */

struct file_link *
get_link_from_dirent (struct dirent *dirent, struct file_link *parent)
{
  struct file_link **slot;
  struct file_link *new_link;

  new_link = make_link_from_dirent (dirent, parent);
  slot = (struct file_link **) hash_find_slot (&idh.idh_file_link_table, new_link);
  if (HASH_VACANT (*slot))
    slot = (struct file_link **) hash_insert_at (&idh.idh_file_link_table,
						 new_link, slot);
  else
    obstack_free (&idh.idh_file_link_obstack, new_link);
  return *slot;
}

struct file_link *
get_link_from_string (char const *name, struct file_link *parent)
{
  struct file_link **slot;
  struct file_link *new_link;

  new_link = make_link_from_string (name, parent);
  slot = (struct file_link **) hash_find_slot (&idh.idh_file_link_table, new_link);
  if (HASH_VACANT (*slot))
    slot = (struct file_link **) hash_insert_at (&idh.idh_file_link_table,
						 new_link, slot);
  else
    obstack_free (&idh.idh_file_link_obstack, new_link);
  return *slot;
}

struct file_link *
make_link_from_dirent (struct dirent* dirent, struct file_link *parent)
{
  struct file_link *flink;

  flink = (struct file_link *) obstack_alloc (&idh.idh_file_link_obstack,
					      sizeof (struct file_link) + strlen (dirent->d_name));
  strcpy (flink->fl_name, dirent->d_name);
  flink->fl_parent = parent ? parent : flink;
  flink->fl_flags = 0;

  return flink;
}

struct file_link *
make_link_from_string (char const* name, struct file_link *parent)
{
  struct file_link *flink;

  flink = (struct file_link *) obstack_alloc (&idh.idh_file_link_obstack,
					      sizeof (struct file_link) + strlen (name));
  strcpy (flink->fl_name, name);
  flink->fl_parent = parent ? parent : flink;
  flink->fl_flags = 0;

  return flink;
}


/****************************************************************************/
/* Convert a `file_link' chain to a vector of component `file_link's,
   with the root at [0].  Return a pointer beyond the final component.  */

struct file_link const **
fill_link_vector (struct file_link const **vec_buf, struct file_link const *flink)
{
  vec_buf = fill_link_vector_1 (vec_buf, flink);
  *vec_buf = 0;
  return vec_buf;
}

struct file_link const **
fill_link_vector_1 (struct file_link const **vec_buf, struct file_link const *flink)
{
  if (!IS_ROOT_FILE_LINK (flink))
    vec_buf = fill_link_vector_1 (vec_buf, flink->fl_parent);
  *vec_buf++ = flink;
  return vec_buf;
}


/****************************************************************************/
/* Fill BUF_0 with a path to TO_LINK.  If a relative path from
   FROM_LINK is possible (i.e., no intervening symbolic-links) and
   shorter, return the relative path; otherwise, return an absolute
   path.  */

char *
maybe_relative_file_name (char *buf_0, struct file_link const *to_link, struct file_link const *from_link)
{
  struct file_link const **to_link_vec_0 = ALLOCA (struct file_link const *, PATH_MAX/2);
  struct file_link const **from_link_vec_0 = ALLOCA (struct file_link const *, PATH_MAX/2);
  struct file_link const **to_link_vec = to_link_vec_0;
  struct file_link const **from_link_vec = from_link_vec_0;
  struct file_link const **from_link_end;
  struct file_link const **from_links;
  int alloc_me = (buf_0 == 0);
  char *buf;
  int levels;

  if (from_link == 0)
    from_link = current_dir_link;

  if (alloc_me)
    buf_0 = MALLOC (char, PATH_MAX);

  /* Optimize common cases.  */
  if (to_link == from_link)
    strcpy (buf_0, ".");
  else if (to_link->fl_parent == from_link)
    strcpy (buf_0, to_link->fl_name);
  else if (from_link->fl_flags & FL_SYM_LINK)
    absolute_file_name (buf_0, to_link);
  else if (to_link == from_link->fl_parent)
    strcpy (buf_0, "..");
  else if (to_link->fl_parent == from_link->fl_parent)
    {
      strcpy (buf_0, DOT_DOT_SLASH);
      strcpy (&buf_0[3], to_link->fl_name);
    }
  else
    {
      from_link_end = fill_link_vector (from_link_vec, from_link);
      fill_link_vector (to_link_vec, to_link);
      while (*to_link_vec == *from_link_vec)
	{
	  if (*to_link_vec == 0)
	    {
	      strcpy (buf_0, ".");
	      goto out;
	    }
	  to_link_vec++;
	  from_link_vec++;
	}
      levels = from_link_end - from_link_vec;
      if (levels >= (from_link_vec - from_link_vec_0))
	{
	  absolute_file_name (buf_0, to_link);
	  goto out;
	}
      for (from_links = from_link_vec; *from_links; from_links++)
	if ((*from_links)->fl_flags & FL_SYM_LINK)
	  {
	    absolute_file_name (buf_0, to_link);
	    goto out;
	  }
      buf = fill_dot_dots (buf_0, levels);
      if (*to_link_vec == 0)
	*--buf = '\0';
      else
	{
	  do
	    {
	      strcpy (buf, (*to_link_vec)->fl_name);
	      buf += strlen (buf);
	      if ((*to_link_vec)->fl_name[0] != SLASH_CHAR && *++to_link_vec)
		*buf++ = SLASH_CHAR;
	    }
	  while (*to_link_vec);
	}
    }
out:
  if (alloc_me)
    buf_0 = REALLOC (buf_0, char, 1 + strlen (buf_0));
  return buf_0;
}

/* Fill `buf' with sequences of "../" in order to ascend so many
   `levels' in a directory tree.  */

char *
fill_dot_dots (char *buf, int levels)
{
  while (levels--)
    {
      strcpy (buf, DOT_DOT_SLASH);
      buf += 3;
    }
  return buf;
}


/****************************************************************************/
/* Fill `buffer' with the absolute path to `flink'.  */

char *
absolute_file_name (char *buf_0, struct file_link const *flink)
{
  char *end;
  int alloc_me = (buf_0 == 0);

  if (alloc_me)
    buf_0 = MALLOC (char, PATH_MAX);
  end = absolute_file_name_1 (buf_0, flink);
  /* Clip the trailing slash.  */
#if HAVE_LINK
  if (end > &buf_0[1])
    end--;
#else
  if (end > &buf_0[3])
    end--;
#endif
  *end++ = '\0';
  if (alloc_me)
    buf_0 = REALLOC (buf_0, char, end - buf_0);
  return buf_0;
}

static char *
absolute_file_name_1 (char *buf, struct file_link const *flink)
{
  char *end;
  if (IS_ROOT_FILE_LINK (flink))
    end = buf;
  else
    end = absolute_file_name_1 (buf, flink->fl_parent);
  strcpy (end, flink->fl_name);
  if (*end == SLASH_CHAR)
    end++;
  else
    {
      end += strlen (end);
      *end++ = SLASH_CHAR;
    }
  return end;
}


/****************************************************************************/
/* Hash stuff for `struct member_file'.  */

unsigned long
member_file_hash_1 (void const *key)
{
  return_ADDRESS_HASH_1 (((struct member_file const *) key)->mf_link);
}

unsigned long
member_file_hash_2 (void const *key)
{
  return_ADDRESS_HASH_2 (((struct member_file const *) key)->mf_link);
}

int
member_file_hash_compare (void const *x, void const *y)
{
  return_ADDRESS_COMPARE (((struct member_file const *) x)->mf_link,
			  ((struct member_file const *) y)->mf_link);
}

/* Collating sequence:
   - language.map index
   - mf_link: breadth-first, alphabetical */

int
member_file_qsort_compare (void const *x, void const *y)
{
  struct member_file const *mfx = *(struct member_file const *const *) x;
  struct member_file const *mfy = *(struct member_file const *const *) y;
  int result;

  INTEGER_COMPARE (mfx->mf_lang_args->la_index, mfy->mf_lang_args->la_index, result);
  if (result)
    return result;
  else
    {
      struct file_link const *flx = mfx->mf_link;
      struct file_link const *fly = mfy->mf_link;
      if (flx->fl_parent == fly->fl_parent)
	return strcmp (flx->fl_name, fly->fl_name);
      result = (links_depth (flx) - links_depth (fly));
      if (result)
	return result;
      while (flx->fl_parent != fly->fl_parent)
	{
	  flx = flx->fl_parent;
	  fly = fly->fl_parent;
	}
      return strcmp (flx->fl_name, fly->fl_name);
    }
}

/****************************************************************************/
/* Hash stuff for `struct file_link'.  */

unsigned long
file_link_hash_1 (void const *key)
{
  unsigned long result = 0;
  struct file_link const *parent = (IS_ROOT_FILE_LINK (((struct file_link const *) key))
				    ? 0 : ((struct file_link const *) key)->fl_parent);
  STRING_HASH_1 (((struct file_link const *) key)->fl_name, result);
  ADDRESS_HASH_1 (parent, result);
  return result;
}

unsigned long
file_link_hash_2 (void const *key)
{
  unsigned long result = 0;
  struct file_link const *parent = (IS_ROOT_FILE_LINK (((struct file_link const *) key))
				    ? 0 : ((struct file_link const *) key)->fl_parent);
  STRING_HASH_2 (((struct file_link const *) key)->fl_name, result);
  ADDRESS_HASH_2 (parent, result);
  return result;
}

int
file_link_hash_compare (void const *x, void const *y)
{
  int result;
  struct file_link const *x_parent = (IS_ROOT_FILE_LINK (((struct file_link const *) x))
				      ? 0 : ((struct file_link const *) x)->fl_parent);
  struct file_link const *y_parent = (IS_ROOT_FILE_LINK (((struct file_link const *) y))
				      ? 0 : ((struct file_link const *) y)->fl_parent);
  ADDRESS_COMPARE (x_parent, y_parent, result);
  if (result)
    return result;
  STRING_COMPARE (((struct file_link const *) x)->fl_name,
		  ((struct file_link const *) y)->fl_name, result);
  return result;
}

/* Count directory components between flink and its root.  */

int
links_depth (struct file_link const *flink)
{
  int depth = 0;
  while (!IS_ROOT_FILE_LINK (flink))
    {
      depth++;
      flink = flink->fl_parent;
    }
  return depth;
}

#if HAVE_LINK

/****************************************************************************/
/* Hash stuff for `struct dev_ino'.  */

unsigned long
dev_ino_hash_1 (void const *key)
{
  unsigned long result = 0;
  INTEGER_HASH_1 (((struct dev_ino const *) key)->di_dev, result);
  INTEGER_HASH_1 (((struct dev_ino const *) key)->di_ino, result);
  return result;
}

unsigned long
dev_ino_hash_2 (void const *key)
{
  unsigned long result = 0;
  INTEGER_HASH_2 (((struct dev_ino const *) key)->di_dev, result);
  INTEGER_HASH_2 (((struct dev_ino const *) key)->di_ino, result);
  return result;
}

int
dev_ino_hash_compare (void const *x, void const *y)
{
  int result;
  INTEGER_COMPARE (((struct dev_ino const *) x)->di_ino,
		   ((struct dev_ino const *) y)->di_ino, result);
  if (result)
    return result;
  INTEGER_COMPARE (((struct dev_ino const *) x)->di_dev,
		   ((struct dev_ino const *) y)->di_dev, result);
  return result;
}

#endif

/*******************************************************************/

struct file_link *
init_walker (struct idhead *idhp)
{
  init_idh_obstacks (idhp);
  init_idh_tables (idhp);
  return get_current_dir_link ();
}

void
init_idh_obstacks (struct idhead *idhp)
{
  obstack_init (&idhp->idh_member_file_obstack);
  obstack_init (&idhp->idh_file_link_obstack);
#if HAVE_LINK
  obstack_init (&idhp->idh_dev_ino_obstack);
#endif
}

void
init_idh_tables (struct idhead *idhp)
{
  hash_init (&idhp->idh_member_file_table, 16*1024,
	     member_file_hash_1, member_file_hash_2, member_file_hash_compare);
  hash_init (&idhp->idh_file_link_table, 16*1024,
	     file_link_hash_1, file_link_hash_2, file_link_hash_compare);
#if HAVE_LINK
  hash_init (&idhp->idh_dev_ino_table, 16*1024,
	     dev_ino_hash_1, dev_ino_hash_2, dev_ino_hash_compare);
#endif
}
