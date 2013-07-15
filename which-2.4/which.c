/*
 * which v2.x -- print full path of executables
 * Copyright (C) 1999  Carlo Wood <carlo@gnu.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "sys.h"
#include <stdio.h>
#include "getopt.h"
#if defined(HAVE_READLINE_TILDE_H) && !defined(NEED_TILDE_EXPAND)
#include <readline/tilde.h>  /* Part of libreadline that comes with binutils */
#else
extern char *tilde_expand (const char *);
#endif
#include "bash.h"

static const char *progname;

static void print_usage(void)
{
  fprintf(stderr, "Usage: %s [options] [--] programname [...]\n", progname);
  fprintf(stderr, "Options: --version, -[vV] Print version and exit successfully.\n");
  fprintf(stderr, "         --skip-dot       Skip directories in PATH that start with a dot.\n");
  fprintf(stderr, "         --skip-tilde     Skip directories in PATH that start with a tilde.\n");
  fprintf(stderr, "         --show-dot       Don't expand a dot to current directory in output.\n");
  fprintf(stderr, "         --show-tilde     Output a tilde for HOME directory for non-root.\n");
  fprintf(stderr, "         --tty-only       Stop processing options on the right if not on tty.\n");
  fprintf(stderr, "         --all, -a        Print all matches in PATH, not just the first\n");
}

static void print_version(void)
{
  fprintf(stdout, "GNU which v" VERSION ", Copyright (C) 1999 Carlo Wood.\n");
  fprintf(stdout, "GNU which comes with ABSOLUTELY NO WARRANTY;\n");
  fprintf(stdout, "This program is free software; your freedom to use, change\n");
  fprintf(stdout, "and distribute this program is covered by the GPL.\n");
}

static void print_fail(const char *name, const char *path_list)
{
  fprintf(stderr, "%s: no %s in (%s)\n", progname, name, path_list);
}

static int absolute_path_given;
static int found_path_starts_with_dot;
static char *abs_path;

static int skip_dot = 0, skip_tilde = 0, show_all = 0;

static char *find_command_in_path(const char *name, const char *path_list, int *path_index)
{
  char *found = NULL, *full_path;
  int status, name_len;

  name_len = strlen(name);

  if (!absolute_program(name))
    absolute_path_given = 0;
  else
  {
    char *p;
    absolute_path_given = 1;

    if (abs_path)
      free(abs_path);

    if (*name != '.' && *name != DIRSEP && *name != '~')
    {
      abs_path = (char *)xmalloc(3 + name_len);
      strcpy(abs_path, "./");
      strcat(abs_path, name);
    }
    else
    {
      abs_path = (char *)xmalloc(1 + name_len);
      strcpy(abs_path, name);
    }

    path_list = abs_path;
    p = strrchr(abs_path, DIRSEP);
    *p++ = 0;
    name = p;
  }

  while (path_list && path_list[*path_index])
  {
    char *path;

    if (absolute_path_given)
    {
      path = savestring(path_list);
      *path_index = strlen(path);
    }
    else
      path = get_next_path_element(path_list, path_index);

    if (!path)
      break;

    if (*path == '~')
    {
      char *t = tilde_expand(path);
      free(path);
      path = t;

      if (skip_tilde)
      {
        free(path);
	continue;
      }
    }

    if (skip_dot && *path != DIRSEP)
    {
      free(path);
      continue;
    }

    found_path_starts_with_dot = (*path == '.');

    full_path = make_full_pathname(path, name, name_len);
    free(path);

    status = file_status(full_path);

    if ((status & FS_EXISTS) && (status & FS_EXECABLE))
    {
      found = full_path;
      break;
    }

    free(full_path);
  }

  return (found);
}

static char cwd[_MAX_DIR];
static size_t cwdlen;

static void get_current_working_directory(void)
{
  if (cwdlen)
    return;

  if (!getcwd(cwd, sizeof(cwd)))
  {
    const char *pwd = getenv("PWD");
    if (pwd && strlen(pwd) < sizeof(cwd))
      strcpy(cwd, pwd);
  }

#ifndef WIN32
  if (*cwd != DIRSEP)
  {
    fprintf(stderr, "Can't get current working directory\n");
    exit(-1);
  }

  cwdlen = strlen(cwd);

  if (cwd[cwdlen - 1] != DIRSEP)
  {
    cwd[cwdlen++] = DIRSEP;
    cwd[cwdlen] = 0;
  }
#else
  cwdlen = strlen(cwd);
#endif
}

static char *path_clean_up(const char *path)
{
  static char result[256];

  const char *p1 = path;
  char *p2 = result;

  int saw_slash = 0, saw_slash_dot = 0, saw_slash_dot_dot = 0;

#ifndef WIN32
  if (*p1 != DIRSEP)
  {
    get_current_working_directory();
    strcpy(result, cwd);
    saw_slash = 1;
    p2 = &result[cwdlen];
  }
#endif

  do
  {
    if (!saw_slash || *p1 != DIRSEP)
      *p2++ = *p1;
    if (saw_slash_dot && (*p1 == DIRSEP))
      p2 -= 2;
    if (saw_slash_dot_dot && (*p1 == DIRSEP))
    {
      int cnt = 0;
      do
      {
	if (--p2 < result)
	{
	  strcpy(result, path);
	  return result;
	}
	if (*p2 == DIRSEP)
	  ++cnt;
      }
      while (cnt != 3);
      ++p2;
    }
    saw_slash_dot_dot = saw_slash_dot && (*p1 == '.');
    saw_slash_dot = saw_slash && (*p1 == '.');
    saw_slash = (*p1 == DIRSEP);
  }
  while (*p1++);

  return result;
}

enum opts {
  opt_version,
  opt_skip_dot,
  opt_skip_tilde,
  opt_show_dot,
  opt_show_tilde,
  opt_tty_only
};

static char home[256];
static size_t homelen = 0;

int main(int argc, char *argv[])
{
  const char *path_list = getenv("PATH");
  int short_option, long_option, fail_count = 0, path_index;
  int show_dot = 0, show_tilde = 0, tty_only = 0;
  struct option longopts[] = {
    {"version", 0, &long_option, opt_version},
    {"skip-dot", 0, &long_option, opt_skip_dot},
    {"skip-tilde", 0, &long_option, opt_skip_tilde},
    {"show-dot", 0, &long_option, opt_show_dot},
    {"show-tilde", 0, &long_option, opt_show_tilde},
    {"tty-only", 0, &long_option, opt_tty_only},
    {"all", 0, NULL, 'a'},
    {NULL, 0, NULL, 0}
  };

  progname = argv[0];
  while ((short_option = getopt_long(argc, argv, "avV", longopts, NULL)) != -1)
  {
    switch (short_option)
    {
      case 0:
	switch (long_option)
	{
	  case opt_version:
	    print_version();
	    return 0;
	  case opt_skip_dot:
	    skip_dot = !tty_only;
	    break;
	  case opt_skip_tilde:
	    skip_tilde = !tty_only;
	    break;
	  case opt_show_dot:
	    show_dot = !tty_only;
	    break;
#ifndef WIN32
	  case opt_show_tilde:
	    show_tilde = (!tty_only && geteuid() != 0);
	    break;
#endif
	  case opt_tty_only:
	    tty_only = !isatty(1);
	    break;
	}
	break;
      case 'a':
        show_all = 1;
        break;
      case 'v':
      case 'V':
	print_version();
	return 0;
    }
  }

  if (show_dot)
    get_current_working_directory();

  if (show_tilde || skip_tilde)
  {
    const char *h;

    if (!(h = getenv("HOME")))
    {
      fprintf(stderr, "%s: ", progname);
      if (show_tilde)
	fprintf(stderr, "--show-tilde");
      else
	fprintf(stderr, "--skip-tilde");
      fprintf(stderr, ": Environment variable HOME not set\n");
      show_tilde = skip_tilde = 0;
    }
    else
    {
      strncpy(home, h, sizeof(home));
      home[sizeof(home) - 1] = 0;
      homelen = strlen(home);
      if (home[homelen - 1] != DIRSEP && homelen < sizeof(home) - 1)
      {
	strcat(home, "/");
	++homelen;
      }
    }
  }

  argv += optind;

  if (!*argv)
  {
    print_usage();
    return -1;
  }

  for (; *argv; ++argv)
  {
    char *result = NULL;
    int found_something = 0;

    if (path_list && *path_list != '\0')
    {
      int next;
      path_index = 0;
      do
      {
        next = show_all;
	result = find_command_in_path(*argv, path_list, &path_index);
	if (result)
	{
	  const char *full_path = path_clean_up(result);
	  int in_home = (show_tilde || skip_tilde) && !strncmp(full_path, home, homelen);
	  if (!(skip_tilde && in_home) && show_dot && found_path_starts_with_dot && !strncmp(full_path, cwd, cwdlen))
	  {
	    full_path += cwdlen;
	    fprintf(stdout, "./");
	  }
	  else if (in_home)
	  {
	    if (skip_tilde)
	    {
	      next = 1;
	      continue;
	    }
	    if (show_tilde)
	    {
	      full_path += homelen;
	      fprintf(stdout, "~/");
	    }
	  }
	  fprintf(stdout, "%s\n", full_path);
	  free(result);
	  found_something = 1;
	}
	else
	  break;
      }
      while(next);
    }
    if (!found_something)
    {
      print_fail(absolute_path_given ? strrchr(*argv, DIRSEP) + 1 : *argv, absolute_path_given ? abs_path : path_list);
      ++fail_count;
    }
  }

  return fail_count;
}

#ifdef NEED_XMALLOC
void *xmalloc(size_t size)
{
  void *ptr = malloc(size);
  if (ptr == NULL)
  {
    fprintf(stderr, "%s: Out of memory", progname);
    exit(-1);
  }
  return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
  if (!ptr)
    return xmalloc(size);
  ptr = realloc(ptr, size);
  if (size > 0 && ptr == NULL)
  {
    fprintf(stderr, "%s: Out of memory\n", progname);
    exit(-1);
  }
  return ptr;
}
#endif /* NEED_XMALLOC */

#ifdef NEED_TILDE_EXPAND
char *tilde_expand (const char *path)
{
  char *ptr;
  if (path[0] == '~')
  {
    if (path[1] == DIRSEP)
    {
      ptr = (char *)xmalloc(homelen + strlen(path));
      strcpy(ptr, home);
      strcat(ptr, &path[2]);
      return ptr;
    }
    fprintf(stderr, "%s: WARNING - this utility was compiled without libreadline, can not expand tilde in \"%s\"\n", progname, path);
  }
  ptr = (char *)xmalloc(strlen(path) + 1);
  strcpy(ptr, path);
  return ptr;
}
#endif /* NEED_TILDE_EXPAND */
