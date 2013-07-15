/* Handle so called `shell archives'.
   Copyright (C) 1994 Free Software Foundation, Inc.
  
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
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "system.h"


static const char *cut_mark_line
  = "---- Cut Here and feed the following to sh ----\n";

/* Delimiter to put after each file.  */
#define	DEFAULT_HERE_DELIMITER "SHAR_EOF"

/* Character which goes in front of each line.  */
#define DEFAULT_LINE_PREFIX_1 'X'

/* Character which goes in front of each line if here_delimiter[0] ==
   DEFAULT_LINE_PREFIX_1.  */
#define DEFAULT_LINE_PREFIX_2 'Y'

/* Shell command able to count characters from its standard input.  */
#define CHARACTER_COUNT_COMMAND "wc -c <"

/* Maximum length for a text line before it is considered binary.  */
#define MAXIMUM_NON_BINARY_LINE 200

/* System related declarations.  */

#include <ctype.h>
#include <time.h>
#include <sys/stat.h>

struct tm *localtime ();

#if !NO_WALKTREE

/* Declare directory reading routines and structures.  */

#ifdef __MSDOS__
# include "msd_dir.h"
# define NAMLEN(dirent) ((dirent)->d_namlen)
#else
# if HAVE_DIRENT_H
#  include <dirent.h>
#  define NAMLEN(dirent) (strlen((dirent)->d_name))
# else
#  define dirent direct
#  define NAMLEN(dirent) ((dirent)->d_namlen)
#  if HAVE_SYS_NDIR_H
#   include <sys/ndir.h>
#  endif
#  if HAVE_SYS_DIR_H
#   include <sys/dir.h>
#  endif
#  if HAVE_NDIR_H
#   include <ndir.h>
#  endif
# endif
#endif

DIR *opendir ();
struct dirent *readdir ();

#endif /* !NO_WALKTREE */

/* Option variables.  */

#include "getopt.h"

/* No Brown-Shirt mode.  */
static int vanilla_operation_mode = 0;

/* Mixed text and binary files.  */
static int mixed_uuencoded_file_mode = -1;

/* Flag for binary files.  */
static int uuencoded_file_mode = -1;

/* Run input files through gzip (requires uuencoded_file_mode).  */
static int gzipped_file_mode = -1;

/* -N option to gzip.  */
static int gzip_compression_level = 9;

/* Run input files through compress (requires uuencoded_file_mode).  */
static int compressed_file_mode = -1;

/* -bN option to compress */
static int bits_per_compressed_byte = 12;

/* Generate $shar_touch commands.  */
static int timestamp_mode = 1;

/* Option to provide wc checking.  */
static int character_count_mode = 1;

/* Use temp file instead of pipe to feed uudecode.  This gives better
   error detection, at expense of disk space.  This is also necessary for
   those versions of uudecode unwilling to read their standard input.  */
static int inhibit_piping_mode = 0;

/* Character to get at the beginning of each line.  */
static int line_prefix;

/* Option to generate "Archive-name:" headers.  */
static int net_headers_mode = 0;

/* Documentation name for archive.  */
static char *archive_name = NULL;

/* Option to provide append feedback at shar time.  */
static int quiet_mode = 0;

/* Option to provide extract feedback at unshar time.  */
static int quiet_unshar_mode = 0;

/* Pointer to delimiter string.  */
static const char *here_delimiter = DEFAULT_HERE_DELIMITER;

/* Value of strlen (here_delimiter).  */
static size_t here_delimiter_length;

/* Use line_prefix even when first char does not force it.  */
static int mandatory_prefix_mode = 0;

/* Option to provide cut mark.  */
static int cut_mark_mode = 0;

/* Check if file exists.  */
static int check_existing_mode = 1;

/* Interactive overwrite.  */
static int query_user_mode = 0;

/* Allow positional parameters.  */
static int intermixed_parameter_mode = 0;

/* Strip directories from filenames.  */
static int basename_mode;

/* Switch for debugging on.  */
#if DEBUG
static int debugging_mode = 0;
#endif

/* Split files in the middle.  */
static int split_file_mode = 0;

/* File size limit in kilobytes.  */
static unsigned file_size_limit = 0;

/* Other global variables.  */

/* The name this program was run with. */
const char *program_name;

/* If non-zero, display usage information and exit.  */
static int show_help = 0;

/* If non-zero, print the version on standard output and exit.  */
static int show_version = 0;

/* File onto which the shar script is being written.  */
static FILE *output = NULL;

/* Position for archive type message.  */
static long archive_type_position;

/* Position for first file in the shar file.  */
static long first_file_position;

/* Base for output filename.  */
static char output_base_name[50];

/* Actual output filename.  */
static char output_filename[50];

static char *submitter_address = NULL;

/* Output file ordinal.  FIXME: also flag for -o.  */
static int part_number = 0;

/* Table saying whether each character is binary or not.  */
static unsigned char byte_is_binary[256];

/* For checking file type and access modes.  */
static struct stat struct_stat;

#if DEBUG
# define DEBUG_PRINT(Format, Value) \
    if (debugging_mode) printf(Format, Value)
#else
# define DEBUG_PRINT(Format, Value)
#endif

static void usage _((int));

/* Walking tree routines.  */

/* Define a type just for easing ansi2knr's life.  */
typedef int (*walker_t) _((const char *, const char *));

#if !NO_WALKTREE

/*--------------------------------------------------------------------------.
| Recursively call ROUTINE on each entry, down the directory tree.  NAME    |
| is the path to explore.  RESTORE_NAME is the name that will be later	    |
| relative to the unsharing directory.  ROUTINE may also assume		    |
| struct_stat is set, it accepts updated values for NAME and RESTORE_NAME.  |
`--------------------------------------------------------------------------*/

static int
walkdown (walker_t routine, const char *local_name, const char *restore_name)
{
  DIR *directory;		/* directory being scanned */
  struct dirent *entry;		/* current entry in directory */
  int status;			/* status to return */

  char *local_name_copy;	/* writeable copy of local_name */
  size_t local_name_length;	/* number of characters in local_name_copy */
  size_t sizeof_local_name;	/* allocated size of local_name_copy */

  char *restore_name_copy;	/* writeable copy of restore_name */
  size_t restore_name_length;	/* number of characters in restore_name_copy */
  size_t sizeof_restore_name;	/* allocated size of restore_name_copy */

  if (stat (local_name, &struct_stat))
    {
      error (0, errno, local_name);
      return 1;
    }

  if (!S_ISDIR (struct_stat.st_mode & S_IFMT))
    return (*routine) (local_name, restore_name);

  if (directory = opendir (local_name), !directory)
    {
      error (0, errno, local_name);
      return 1;
    }

  status = 0;

  local_name_copy = xstrdup (local_name);
  local_name_length = strlen (local_name_copy);
  sizeof_local_name = local_name_length + 1;

  restore_name_copy = xstrdup (restore_name);
  restore_name_length = strlen (restore_name_copy);
  sizeof_restore_name = restore_name_length + 1;

  while (!status && (entry = readdir (directory), entry))
    if (strcmp (entry->d_name, ".") && strcmp (entry->d_name, ".."))
      {
	int added_size = 1 + NAMLEN (entry);

	/* Update file names, reallocating them as required.  */

	if (local_name_length + added_size + 1 > sizeof_local_name)
	  {
	    sizeof_local_name = local_name_length + added_size + 1;
	    local_name_copy = (char *)
	      xrealloc (local_name_copy, sizeof_local_name);
	  }
	sprintf (local_name_copy + local_name_length, "/%s", entry->d_name);

	if (restore_name_length + added_size + 1 > sizeof_restore_name)
	  {
	    sizeof_restore_name = restore_name_length + added_size + 1;
	    restore_name_copy = (char *)
	      xrealloc (restore_name_copy, sizeof_restore_name);
	  }
	sprintf (restore_name_copy + restore_name_length, "/%s",
		 entry->d_name);

	/* Avoid restoring `./xxx' when shar'ing `.'.  */

	if (!strncmp (restore_name, "./", 2))
	  {
	    strcpy (restore_name_copy, restore_name_copy + 2);
	    restore_name_length -= 2;
	  }

	status = walkdown (routine, local_name_copy, restore_name_copy);
      }

  /* Clean up.  */

  if (sizeof_local_name > 0)
    free (local_name_copy);
  if (sizeof_restore_name > 0)
    free (restore_name_copy);

#if CLOSEDIR_VOID
  closedir (directory);
#else
  if (closedir (directory))
    {
      error (0, errno, local_name);
      return 1;
    }
#endif

  return status;
}

#endif /* !NO_WALKTREE */

/*------------------------------------------------------------------.
| Walk through the directory tree, calling ROUTINE for each entry.  |
| ROUTINE may also assume struct_stat is set.			    |
`------------------------------------------------------------------*/

static int
walktree (walker_t routine, const char *local_name)
{
  const char *restore_name;
  char *local_name_copy;
  char *cursor;
  int status;

  /* Remove crumb at end.  */

  local_name_copy = xstrdup (local_name);
  cursor = local_name_copy + strlen (local_name_copy) - 1;
  while (*cursor == '/' && cursor > local_name_copy)
    *cursor-- = '\0';

  /* Remove crumb at beginning.  */

  if (basename_mode)
    restore_name = basename (local_name_copy);
  else if (!strncmp (local_name_copy, "./", 2))
    restore_name = local_name_copy + 2;
  else
    restore_name = local_name_copy;

#if NO_WALKTREE

  /* Just act on current entry.  */

  if (status = stat (local_name_copy, &struct_stat), status)
    error (0, errno, local_name_copy);
  else
    status = (*routine) (local_name_copy, restore_name);

#else

  /* Walk recursively.  */

  status = walkdown (routine, local_name_copy, restore_name);

#endif

  free (local_name_copy);
  return status;
}

/* Generating parts of shar file.  */

/*---------------------------------------------------------------------.
| Build a `drwxrwxrwx' string corresponding to MODE into MODE_STRING.  |
`---------------------------------------------------------------------*/

static char *
mode_string (unsigned mode)
{
  static char result[12];

  strcpy (result, "----------");

  if (mode & 00400)
    result[1] = 'r';
  if (mode & 00200)
    result[2] = 'w';
  if (mode & 00100)
    result[3] = 'x';
  if (mode & 04000)
    result[3] = 's';
  if (mode & 00040)
    result[4] = 'r';
  if (mode & 00020)
    result[5] = 'w';
  if (mode & 00010)
    result[6] = 'x';
  if (mode & 02000)
    result[6] = 's';
  if (mode & 00004)
    result[7] = 'r';
  if (mode & 00002)
    result[8] = 'w';
  if (mode & 00001)
    result[9] = 'x';

  return result;
}

/*-----------------------------------------------------------------------.
| Generate shell code which, at *unshar* time, will study the properties |
| of the unpacking system and set some variables accordingly.		 |
`-----------------------------------------------------------------------*/

static void
generate_configure (void)
{
  if (query_user_mode)
    if (vanilla_operation_mode)
      fputs ("\
shar_tty= shar_n= shar_c='\n\
'\n",
	     output);
    else
      {

	/* Check if /dev/tty exists.  If yes, define shar_tty to
	   `/dev/tty', else, leave it empty.  */

	fputs ("\
if test -n \"`ls /dev/tty 2>/dev/null`\"; then\n\
  shar_tty=/dev/tty\n\
else\n\
  shar_tty=\n\
fi\n",
	       output);

	/* Try to find a way to echo a message without newline.  Set
	   shar_n to `-n' or nothing for an echo option, and shar_c to
	   `\c' or nothing for a string terminator.  */

	fputs ("\
if (echo \"testing\\c\"; echo 1,2,3) | grep c >/dev/null; then\n\
  if (echo -n testing; echo 1,2,3) | sed s/-n/xn/ | grep xn >/dev/null; then\n\
    shar_n= shar_c='\n\
'\n\
  else\n\
    shar_n=-n shar_c=\n\
  fi\n\
else\n\
  shar_n= shar_c='\\c'\n\
fi\n",
	       output);
      }

  if (timestamp_mode)
    {
      const char *file = "$$.touch";
      const char *stamp = "1231235999";

      /* Set the shell variable shar_touch to `touch' if the touch
	 program is proven able to restore dates.  Otherwise, set
	 shar_touch to `:' and issue a warning.  */

      fprintf (output, "\
touch -am %s %s >/dev/null 2>&1\n\
if test ! -f %s && test -f %s; then\n\
  shar_touch=touch\n\
else\n\
  shar_touch=:\n\
  echo\n\
  echo 'WARNING: not restoring timestamps.  Consider getting and'\n\
  echo \"installing GNU \\`touch', distributed in GNU File Utilities...\"\n\
  echo\n\
fi\n\
rm -f %s %s\n\
#\n",
	       stamp, file, stamp, file, stamp, file);
    }
}

/*---.
| ?  |
`---*/

/* Ridiculously enough.  */
#define MAX_MKDIR_ALREADY	128

char *mkdir_already[MAX_MKDIR_ALREADY];
int mkdir_already_count = 0;

static void
generate_mkdir (const char *path)
{
  int counter;
  char *cursor;

  /* If already generated code for this dir creation, don't do again.  */

  for (counter = 0; counter < mkdir_already_count; counter++)
    if (!strcmp (path, mkdir_already[counter]))
      return;

  /* Haven't done this one.  */

  if (mkdir_already_count == MAX_MKDIR_ALREADY)
    error (EXIT_FAILURE, 0, "Too many directories for mkdir generation");
  cursor = mkdir_already[mkdir_already_count++] = xmalloc (strlen (path) + 1);
  strcpy (cursor, path);

  /* Generate the text.  */

  fprintf (output, "if test ! -d '%s'; then\n", path);
  if (!quiet_unshar_mode)
    fprintf (output, "  echo 'x - creating directory %s'\n", path);
  fprintf (output, "  mkdir '%s'\n", path);
  fputs ("fi\n", output);
}

/*---.
| ?  |
`---*/

static void
generate_mkdir_script (const char *path)
{
  char *cursor;

  for (cursor = strchr (path, '/'); cursor; cursor = strchr (cursor + 1, '/'))
    {

      /* Avoid empty string if leading or double '/'.  */

      if (cursor == path || *(cursor - 1) == '/')
	continue;

      /* Omit '.'.  */

      if (cursor[-1] == '.' && (cursor == path + 1 || cursor[-2] == '/'))
	continue;

      /* Temporarily terminate string.  FIXME!  */

      *cursor = 0;
      generate_mkdir (path);
      *cursor = '/';
    }
}

/* Walking routines.  */

/*---.
| ?  |
`---*/

static int
check_accessibility (const char *local_name, const char *restore_name)
{
  if (access (local_name, 4))
    {
      error (0, 0, "Cannot access %s", local_name);
      return 1;
    }

  return 0;
}

/*---.
| ?  |
`---*/

static int
generate_one_header_line (const char *local_name, const char *restore_name)
{
  fprintf (output, "# %6ld %s %s\n", struct_stat.st_size,
	   mode_string (struct_stat.st_mode), restore_name);
  return 0;
}

/*---.
| ?  |
`---*/

static void
generate_full_header (int argc, char *const *argv)
{
  char *current_directory;
  time_t now;
  struct tm *local_time;
  char buffer[80];
  int warned_once;
  int counter;

  warned_once = 0;
  for (counter = 0; counter < argc; counter++)
    {

      /* Skip positional parameters.  */

      if (intermixed_parameter_mode &&
	  (strcmp (argv[counter], "-B") == 0 ||
	   strcmp (argv[counter], "-T") == 0 ||
	   strcmp (argv[counter], "-M") == 0 ||
	   strcmp (argv[counter], "-z") == 0 ||
	   strcmp (argv[counter], "-Z") == 0 ||
	   strcmp (argv[counter], "-C") == 0))
	{
	  if (!warned_once && strcmp (argv[counter], "-C") == 0)
	    {
	      error (0, 0, "-C is being deprecated, use -Z instead");
	      warned_once = 1;
	    }
	  continue;
	}

      if (walktree (check_accessibility, argv[counter]))
	exit (EXIT_FAILURE);
    }

  if (net_headers_mode)
    {
      fprintf (output, "Submitted-by: %s\n", submitter_address);
      fprintf (output, "Archive-name: %s%s%02d\n\n",
	       archive_name, (strchr (archive_name, '/')) ? "" : "/part",
	       part_number ? part_number : 1);
    }

  if (cut_mark_mode)
    fputs (cut_mark_line, output);
  fputs ("\
#!/bin/sh\n",
	 output);
  if (archive_name)
    fprintf (output, "\
# This is %s, a shell archive (produced by GNU %s %s)\n",
	     archive_name, PRODUCT, VERSION);
  else
    fprintf (output, "\
# This is a shell archive (produced by GNU %s %s).\n",
	     PRODUCT, VERSION);
  fputs ("\
# To extract the files from this archive, save it to some FILE, remove\n\
# everything before the `!/bin/sh' line above, then type `sh FILE'.\n\
#\n",
	 output);

  time (&now);
  local_time = localtime (&now);
  strftime (buffer, 79, "%Y-%m-%d %H:%M %Z", local_time);
  fprintf (output, "\
# Made on %s by <%s>.\n",
	   buffer, submitter_address);

  current_directory = xgetcwd ();
  if (current_directory)
    {
      fprintf (output, "\
# Source directory was `%s'.\n",
	       current_directory);
      free (current_directory);
    }
  else
    error (0, errno, "Cannot get current directory name");

  if (check_existing_mode)
    fputs ("\
#\n\
# Existing files will *not* be overwritten unless `-c' is specified.\n",
	   output);
  else if (query_user_mode)
    fputs ("\
#\n\
# existing files MAY be overwritten\n",
	   output);
  else
    fputs ("\
#\n\
# existing files WILL be overwritten\n",
	   output);

  if (query_user_mode)
    fputs ("\
# The unsharer will be INTERACTIVELY queried.\n",
	   output);

  if (vanilla_operation_mode)
    {
      fputs ("\
# This format requires very little intelligence at unshar time.\n\
# ",
	     output);
      if (check_existing_mode || split_file_mode)
	fputs ("\"if test\", ", output);
      if (split_file_mode)
	fputs ("\"cat\", \"rm\", ", output);
      fputs ("\"echo\", and \"sed\" may be needed.\n", output);
    }

  if (split_file_mode)
    {

      /* May be split, explain.  */

      fputs ("#\n", output);
      archive_type_position = ftell (output);
      fprintf (output, "%-75s\n%-75s\n", "#", "#");
    }

  fputs ("\
#\n\
# This shar contains:\n\
# length mode       name\n\
# ------ ---------- ------------------------------------------\n",
	 output);

  for (counter = 0; counter < argc; counter++)
    {

      /* Output names of files but not parameters.  */

      if (intermixed_parameter_mode &&
	  (strcmp (argv[counter], "-B") == 0 ||
	   strcmp (argv[counter], "-T") == 0 ||
	   strcmp (argv[counter], "-M") == 0 ||
	   strcmp (argv[counter], "-z") == 0 ||
	   strcmp (argv[counter], "-Z") == 0 ||
	   strcmp (argv[counter], "-C") == 0))
	continue;

      if (walktree (generate_one_header_line, argv[counter]))
	exit (EXIT_FAILURE);
    }
  fputs ("#\n", output);

  generate_configure ();

  if (split_file_mode)
    {

      /* Now check the sequence.  */

      fputs ("\
if test -r _sharseq.tmp; then\n\
  echo 'Must unpack archives in sequence!'\n\
  echo Please unpack part `cat _sharseq.tmp` next\n\
  exit 1\n\
fi\n",
	     output);
    }
}

/* Prepare a shar script.  */

/*---.
| ?  |
`---*/

static int
shar (const char *local_name, const char *restore_name)
{
  char buffer[BUFSIZ];
  FILE *input;
  long current_size;
  long remaining_size;
  int split_flag = 0;		/* file split flag */
  const char *file_type;	/* text or binary */
  struct tm *restore_time;

  /* Check to see that this is still a regular file and readable.  */

  if (!S_ISREG (struct_stat.st_mode & S_IFMT))
    {
      error (0, 0, "%s: Not a regular file", local_name);
      return 1;
    }
  if (access (local_name, 4))
    {
      error (0, 0, "Cannot access %s", local_name);
      return 1;
    }

  /* If file_size_limit set, get the current output length.  */

  if (file_size_limit)
    {
      current_size = ftell (output);
      remaining_size = (file_size_limit * 1024L) - current_size;
      DEBUG_PRINT ("In shar: remaining size %ld\n", remaining_size);

      if (!split_file_mode && current_size > first_file_position
	  && ((uuencoded_file_mode
	       ? struct_stat.st_size + struct_stat.st_size / 3
	       : struct_stat.st_size)
	      > remaining_size))
	{

	  /* Change to another file.  */

	  DEBUG_PRINT ("Newfile, remaining %ld, ", remaining_size);
	  DEBUG_PRINT ("limit still %d\n", file_size_limit);

	  /* Close the "&&" and report an error if any of the above
	     failed.  */

	  fprintf (output, "\
: || echo 'restore of %s failed'\n\
echo 'End of part %d, continue with part %d'\n\
exit 0\n",
		   restore_name, part_number, part_number + 1);

	  fclose (output);

	  /* Clear mkdir_already in case the user unshars out of order.  */

	  while (mkdir_already_count > 0)
	    free (mkdir_already[--mkdir_already_count]);

	  /* Form the next filename.  */

	  sprintf (output_filename, "%s%02d", output_base_name, ++part_number);
	  output = fopen (output_filename, "w");
	  if (!quiet_mode)
	    fprintf (stderr, "Starting file %s\n", output_filename);

	  if (net_headers_mode)
	    {
	      fprintf (output, "Submitted-by: %s\n", submitter_address);
	      fprintf (output, "Archive-name: %s%s%02d\n\n", archive_name,
		       strchr (archive_name, '/') ? "" : "/part",
		       part_number ? part_number : 1);
	    }

	  if (cut_mark_mode)
	    fputs (cut_mark_line, output);

	  fprintf (output, "\
#!/bin/sh\n\
# This is part %02d of %s.\n",
		   part_number,
		   archive_name ? archive_name : "a multipart archive");

	  generate_configure ();

	  first_file_position = ftell (output);
	}
    }
  else
    remaining_size = 0;		/* give some value to the variable */

  fprintf (output, "\
# ============= %s ==============\n",
	   restore_name);

  generate_mkdir_script (restore_name);

  if (struct_stat.st_size == 0)
    {
      file_type = "empty";
      input = NULL;		/* give some value to the variable */
    }
  else
    {

      /* If mixed, determine the file type.  */

      if (mixed_uuencoded_file_mode) 
	{

	  /* Uuencoded was once decided through calling the `file'
	     program and studying its output: the method was slow and
	     error prone.  There is only one way of doing it correctly,
	     and this is to read the input file, seeking for one binary
	     character.  Considering the average file size, even reading
	     the whole file (if it is text) would be usually faster than
	     calling `file'.  */

	  int character;
	  int line_length;
	  

	  if (input = fopen (local_name, "rb"), input == NULL)
	    {
	      error (0, errno, "Cannot open file %s", local_name);
	      return 1;
	    }

	  /* Assume initially that the input file is text.  Then try to prove
	     it is binary by looking for binary characters or long lines.  */

	  uuencoded_file_mode = 0;
	  line_length = 0;
	  while (character = getc (input), character != EOF)
	    if (character == '\n')
	      line_length = 0;
	    else if (
#ifdef __CHAR_UNSIGNED__
		     byte_is_binary[character]
#else
		     byte_is_binary[character & 0xFF]
#endif
		     || line_length == MAXIMUM_NON_BINARY_LINE)
	      {
		uuencoded_file_mode = 1;
		break;
	      }
	    else
	      line_length++;
	  fclose (input);

	  /* Text files should terminate by an end of line.  */

	  if (line_length > 0)
	    uuencoded_file_mode = 1;
	}

      if (uuencoded_file_mode) 
	{
	  static int pid, pipex[2];

	  file_type = (compressed_file_mode ? "compressed"
		       : gzipped_file_mode ? "gzipped" : "binary");

	  /* Fork a uuencode process.  */

	  pipe (pipex);
	  fflush (output);

	  if (pid = 1 /* fork ()*/ , pid != 0)
	    {

	      /* Parent, create a file to read.  */

	      if (pid < 0)
		error (EXIT_FAILURE, errno, "Could not fork");
//	      close (pipex[1]);
	      input = fdopen (pipex[0], "r");
	      if (!input)
		{
		  error (0, errno, "File %s (%s)", local_name, file_type);
		  return 1;
		}

	    }
	  
//	  else
	    {

	      /* Start writing the pipe with encodes.  */

	      FILE *outptr,*inputp;

	      if (compressed_file_mode)
		{
		  sprintf (buffer, "compress -b%d < %s",
			   bits_per_compressed_byte, local_name);
		  inputp = popen (buffer, "r");
		}
	      else if (gzipped_file_mode)
		{
		  sprintf (buffer, "gzip -%d < %s",
			   gzip_compression_level, local_name);
		  inputp = popen (buffer, "r");
		}
	      else
		inputp = fopen (local_name, "rb");

	      outptr = fdopen (pipex[1], "w");
	      fprintf (outptr, "begin 600 %s\n",
		       (compressed_file_mode ? "_sharcmp.tmp"
			: gzipped_file_mode ? "_shargzi.tmp" : restore_name));
	      copy_file_encoded (inputp, outptr);
	      fprintf (outptr, "end\n");
	      if (compressed_file_mode || gzipped_file_mode)
		pclose (inputp);
	      else
		fclose (inputp);


//	      exit (EXIT_SUCCESS);
	    }
	}
      else
	{
	  file_type = "text";

	  input = fopen (local_name, "r");
	  if (!input)
	    {
	      error (0, errno, "File %s (%s)", local_name, file_type);
	      return 1;
	    }
	}
    }

  /* Protect existing files.  */

  if (check_existing_mode)
    {
      fprintf (output, "\
if test -f '%s' && test X\"$1\" != X\"-c\"; then\n",
	       restore_name);
	  
      if (query_user_mode)
	{
	  fprintf (output, "\
  case $shar_wish in\n\
    A*|a*)\n\
      echo 'x - overwriting %s' ;;\n\
    *)\n\
      echo $shar_n \"? - overwrite %s [no, yes, all, quit] (no)? $shar_c\"\n\
      if test -n \"$shar_tty\"; then\n\
	read shar_wish < $shar_tty\n\
      else\n\
	read shar_wish\n\
      fi ;;\n\
  esac\n\
  case $shar_wish in\n\
    Q*|q*)\n\
      echo 'extraction aborted'; exit 1 ;;\n\
    A*|a*|Y*|y*)\n\
      shar_skip=no ;;\n\
    *)\n\
      shar_skip=yes ;;\n\
  esac\n\
else\n\
  shar_skip=no\n\
fi\n\
if test $shar_skip = yes; then\n\
  echo 'x - skipping %s'\n",
		   restore_name, restore_name, restore_name);
	}
      else
	fprintf (output, "\
  echo 'x - skipping %s (file already exists)'\n",
		 restore_name);

      if (split_file_mode)
	fputs ("\
  rm -f _sharnew.tmp\n",
	       output);

      fputs ("\
else\n",
	     output);

      if (split_file_mode)
	fputs ("\
  > _sharnew.tmp\n",
	       output);
    }

  error (0, 0, "Saving %s (%s)", local_name, file_type);
      
  if (!quiet_unshar_mode)
    fprintf (output, "\
  echo 'x - extracting %s (%s)'\n",
	     restore_name, file_type);

  if (struct_stat.st_size == 0)
    {

      /* Just touch the file, or empty it if it exists.  */

      fprintf (output, "\
  > '%s' &&\n",
	       restore_name);
    }
  else
    {

      /* Run sed for non-empty files.  */

      if (uuencoded_file_mode)
	{

	  /* Run sed through uudecode (via temp file if might get split).  */

	  fprintf (output, "\
  sed 's/^%c//' << '%s' %s &&\n",
		   line_prefix, here_delimiter,
		   inhibit_piping_mode ? "> _sharuue.tmp" : "| uudecode");
	}
      else
	{

	  /* Just run it into the file.  */

	  fprintf (output, "\
  sed 's/^%c//' << '%s' > '%s' &&\n",
		   line_prefix, here_delimiter, restore_name);
	}

      while (fgets (buffer, BUFSIZ, input))
	{

	  /* Output a line and test the length.  */

	  if (!mandatory_prefix_mode

#ifdef isgraph
	      && isgraph (buffer[0])
#else
	      && isprint (buffer[0]) && !isspace (buffer[0])
#endif

	      /* Protect lines already starting with the prefix.  */
	      && buffer[0] != line_prefix

	      /* Old mail programs interpret ~ directives.  */
	      && buffer[0] != '~'

	      /* Avoid mailing lines which are just `.'.  */
	      && buffer[0] != '.'

#if STRNCMP_IS_FAST
	      && strncmp (buffer, here_delimiter, here_delimiter_length)

	      /* unshar -e: avoid `exit 0'.  */
	      && strncmp (buffer, "exit 0", 6)

	      /* Don't let mail prepend a `>'.  */
	      && strncmp (buffer, "From", 4)
#else
	      && (buffer[0] != here_delimiter[0]
		  || strncmp (buffer, here_delimiter, here_delimiter_length))

	      /* unshar -e: avoid `exit 0'.  */
	      && (buffer[0] != 'e' || strncmp (buffer, "exit 0", 6))

	      /* Don't let mail prepend a `>'.  */
	      && (buffer[0] != 'F' || strncmp (buffer, "From", 4))
#endif
	      )
	    fputs (buffer, output);
	  else
	    {
	      fprintf (output, "%c%s", line_prefix, buffer);
	      remaining_size--;
	    }

	  /* Try completing an incomplete line, but not if the incomplete
	     line contains no character.  This might occur with -T for
	     incomplete files, or sometimes when switching to a new file.  */

	  if (*buffer && buffer[strlen (buffer) - 1] != '\n')
	    {
	      fputc ('\n', output);
	      remaining_size--;
	    }

	  if (split_file_mode
#if MSDOS
	      /* 1 extra for CR.  */
	      && (remaining_size -= (int) strlen (buffer) + 1) < 0
#else
	      && (remaining_size -= (int) strlen (buffer)) < 0
#endif
	      )
	    {

	      /* Change to another file.  */

	      DEBUG_PRINT ("Newfile, remaining %ld, ", remaining_size);
	      DEBUG_PRINT ("limit still %d\n", file_size_limit);

	      fprintf (output, "%s\n", here_delimiter);

	      /* Close the "&&" and report an error if any of the above
		 failed.  */

	      fprintf (output, "\
  : || echo 'restore of %s failed'\n",
		       restore_name);

	      if (check_existing_mode)
		fputs ("\
fi\n",
		       output);

	      if (quiet_unshar_mode)
		fprintf (output, "\
echo 'End of part %d, continue with part %d'\n",
			 part_number, part_number + 1);
	      else
		fprintf (output, "\
echo 'End of %s part %d'\n\
echo 'File %s is continued in part %d'\n",
			 archive_name ? archive_name : "archive",
			 part_number, restore_name, part_number + 1);

	      fprintf (output, "\
echo %d > _sharseq.tmp\n\
exit 0\n",
		       part_number + 1);

	      if (part_number == 1)
		{

		  /* Rewrite the info lines on the first header.  */

		  fseek (output, archive_type_position, 0);
		  fprintf (output, "%-75s\n%-75s\n",
			   "\
# This is part 1 of a multipart archive.",
			   "\
# Do not concatenate these parts, unpack them in order with `/bin/sh'.");
			   }
	      fclose (output);

	      /* Form the next filename.  */

	      sprintf (output_filename, "%s%02d",
		       output_base_name, ++part_number);
	      output = fopen (output_filename, "w");

	      if (net_headers_mode)
		{
		  fprintf (output, "Submitted-by: %s\n", submitter_address);
		  fprintf (output, "Archive-name: %s%s%02d\n\n",
			   archive_name,
			   strchr (archive_name, '/') ? "" : "/part",
			   part_number ? part_number : 1);
		}

	      if (cut_mark_mode)
		fputs (cut_mark_line, output);

	      fprintf (output, "\
#!/bin/sh\n\
# This is `%s' (part %d of %s).\n\
# Do not concatenate these parts, unpack them in order with `/bin/sh'.\n\
# File `%s' is being continued...\n\
#\n",
		       basename (output_filename), part_number,
		       archive_name ? archive_name : "a multipart archive",
		       restore_name);

	      generate_configure ();

	      fprintf (output, "\
if test ! -r _sharseq.tmp; then\n\
  echo 'Please unpack part 1 first!'\n\
  exit 1\n\
fi\n\
shar_sequence=`cat _sharseq.tmp`\n\
if test \"$shar_sequence\" != %d; then\n\
  echo \"Please unpack part $shar_sequence next!\"\n\
  exit 1\n\
fi\n",
		       part_number);

	      if (check_existing_mode)
		if (quiet_unshar_mode)
		  fputs ("\
if test -f _sharnew.tmp; then\n",
			 output);
		else
		  fprintf (output, "\
if test ! -f _sharnew.tmp; then\n\
  echo 'x - still skipping %s'\n\
else\n",
			   restore_name);

	      if (!quiet_mode)
		fprintf (stderr, "Starting file %s\n", output_filename);
	      if (!quiet_unshar_mode)
		fprintf (output, "\
  echo 'x - continuing file %s'\n",
			 restore_name);
	      fprintf (output, "\
  sed 's/^%c//' << '%s' >> '%s' &&\n",
		       line_prefix, here_delimiter,
		       uuencoded_file_mode ? "_sharuue.tmp" : restore_name);
	      remaining_size = file_size_limit * 1024L;
	      split_flag = 1;
	    }
	}

      fclose (input);
      while (wait (NULL) >= 0)
	;

      fprintf (output, "%s\n", here_delimiter);
      if (split_flag && !quiet_unshar_mode)
	fprintf (output, "\
  echo 'File %s is complete' &&\n", restore_name);

      /* If this file was uuencoded w/Split, decode it and drop the temp.  */

      if (uuencoded_file_mode && inhibit_piping_mode)
	{
	  if (!quiet_unshar_mode)
	    fprintf (output, "\
  echo 'uudecoding file %s' &&\n",
		     restore_name);

	  fputs ("\
  uudecode _sharuue.tmp < _sharuue.tmp && rm -f _sharuue.tmp &&\n",
		 output);
	}

      /* If this file was compressed, uncompress it and drop the temp.  */

      if (compressed_file_mode)
	{
	  if (!quiet_unshar_mode)
	    fprintf (output, "\
  echo 'uncompressing file %s' &&\n",
		     restore_name);

	  fprintf (output, "\
  compress -d < _sharcmp.tmp > '%s' && rm -f _sharcmp.tmp &&\n",
		   restore_name);
	}
      else if (gzipped_file_mode)
	{
	  if (!quiet_unshar_mode)
	    fprintf (output, "\
  echo 'gunzipping file %s' &&\n",
		     restore_name);

	  fprintf (output, "\
  gzip -d < _shargzi.tmp > '%s' && rm -f _shargzi.tmp &&\n",
		   restore_name);
	}
    }

  if (timestamp_mode)
    {

      /* Set the dates as they were.  */

      restore_time = localtime (&struct_stat.st_mtime);
      fprintf (output, "\
  $shar_touch -am %02d%02d%02d%02d%02d '%s' &&\n",
	       restore_time->tm_mon + 1, restore_time->tm_mday,
	       restore_time->tm_hour, restore_time->tm_min,
	       restore_time->tm_year, restore_name);
    }

  if (vanilla_operation_mode)
    {

      /* Close the "&&" and report an error if any of the above
	 failed.  */

      fprintf (output, "\
  : || echo 'restore of %s failed'\n",
	       restore_name);
    }
  else
    {

      /* Set the permissions as they were.  */

      fprintf (output, "\
  chmod %04o '%s' ||\n",
	       (unsigned) (struct_stat.st_mode & 0777), restore_name);

      /* Report an error if any of the above failed.  */

      fprintf (output, "\
  echo 'restore of %s failed'\n",
	       restore_name);

      if (character_count_mode)
	{

	  /* Validate the transferred file.  */

	  FILE *pfp;
	  char command[BUFSIZ];

	  sprintf (command, "%s %s", CHARACTER_COUNT_COMMAND, local_name);

	  if (pfp = popen (command, "r"), pfp)
	    {
	      char wc[BUFSIZ];

		      fscanf (pfp, "%s", wc);
	      fprintf (output, "\
  shar_count=\"`%s '%s'`\"\n\
  test %s -eq \"$shar_count\" ||\n\
    echo \"%s: original size %s, current size $shar_count\"\n",
		       CHARACTER_COUNT_COMMAND, restore_name,
		       wc, restore_name, wc);
	      pclose (pfp);
	    }
	}
    }

  /* If the exists option is in place close the if.  */

  if (check_existing_mode)
    {
      if (split_file_mode)
	fputs ("\
  rm -f _sharnew.tmp\n",
	       output);

      fputs ("\
fi\n",
	     output);
    }

  return 0;
}

/* Main control.  */

/*-----------------------------------------------------------------------.
| Set file mode, accepting a parameter 'M' for mixed uuencoded mode, 'B' |
| for uuencoded mode, 'z' for gzipped mode or 'Z' for compressed mode.	 |
| Any other value yields text mode.					 |
`-----------------------------------------------------------------------*/

static void
set_file_mode (int mode)
{
  mixed_uuencoded_file_mode = mode == 'M';
  uuencoded_file_mode = mode == 'B';
  gzipped_file_mode = mode == 'z';
  compressed_file_mode = mode == 'Z';

  if (gzipped_file_mode || compressed_file_mode)
    uuencoded_file_mode = 1;
}

/*----------------------------------.
| Output a command format message.  |
`----------------------------------*/

static void
usage (int status)
{
  if (status != EXIT_SUCCESS)
    fprintf (stderr, "Try `%s --help' for more information.\n", program_name);
  else
    {
      printf ("Usage: %s [OPTION]... [FILE]...\n", program_name);
      fputs ("\
Mandatory arguments to long options are mandatory for short options too.\n",
	     stdout);
      fputs ("\
\n\
Giving feedback:\n\
      --help              display this help and exit\n\
      --version           output version information and exit\n\
  -q, --quiet, --silent   do not output verbose messages locally\n\
\n\
Selecting files:\n\
  -p, --intermix-type     allow -[BTzZ] in file lists to change mode\n\
  -S, --stdin-file-list   read file list from standard input\n\
\n\
Splitting output:\n\
  -o, --output-prefix=PREFIX    output to file PREFIX.01 through PREFIX.NN\n\
  -l, --whole-size-limit=SIZE   split archive, not files, to SIZE kilobytes\n\
  -L, --split-size-limit=SIZE   split archive, or files, to SIZE kilobytes\n",
	     stdout);
      fputs ("\
\n\
Controlling the shar headers:\n\
  -n, --archive-name=NAME   use NAME to document the archive\n\
  -s, --submitter=ADDRESS   override the submitter name\n\
  -a, --net-headers         output Submitted-by: & Archive-name: headers\n\
  -c, --cut-mark            start the shar with a cut line\n\
\n\
Selecting how files are stocked:\n\
  -M, --mixed-uuencode         dynamically decide uuencoding (default)\n\
  -T, --text-files             treat all files as text\n\
  -B, --uuencode               treat all files as binary, use uuencode\n\
  -z, --gzip                   gzip and uuencode all files\n\
  -g, --level-for-gzip=LEVEL   pass -LEVEL (default 9) to gzip\n\
  -Z, --compress               compress and uuencode all files\n\
  -b, --bits-per-code=BITS     pass -bBITS (default 12) to compress\n",
	     stdout);
      fputs ("\
\n\
Protecting against transmission:\n\
  -w, --no-character-count      do not use `wc -c' to check size\n\
  -F, --force-prefix            force the prefix character on every line\n\
  -d, --here-delimiter=STRING   use STRING to delimit the files in the shar\n\
\n\
Producing different kinds of shars:\n\
  -V, --vanilla-operation   produce very simple and undemanding shars\n\
  -P, --no-piping           exclusively use temporary files at unshar time\n\
  -x, --no-check-existing   blindly overwrite existing files\n\
  -X, --query-user          ask user before overwriting files (not for Net)\n\
  -m, --no-timestamp        do not restore file modification dates & times\n\
  -Q, --quiet-unshar        avoid verbose messages at unshar time\n\
  -f, --basename            restore all in one directory, despite hierarchy\n",
	     stdout);
      fputs ("\
\n\
Option -o is required with -l or -L, option -n is required with -a.\n\
Option -g implies -z, option -b implies -Z.\n",
	     stdout);
    }
  exit (status);
}

/*--------------------------------------.
| Decode options and launch execution.  |
`--------------------------------------*/

static const struct option long_options[] =
{
  {"archive-name", required_argument, NULL, 'n'},
  {"basename", no_argument, NULL, 'f'}, 
  {"bits-per-code", required_argument, NULL, 'b'},
  {"compress", no_argument, NULL, 'Z'}, 
  {"cut-mark", no_argument, NULL, 'c'}, 
  {"force-prefix", no_argument, NULL, 'F'}, 
  {"gzip", no_argument, NULL, 'z'}, 
  {"here-delimiter", required_argument, NULL, 'd'},
  {"intermix-type", no_argument, NULL, 'p'}, 
  {"level-for-gzip", required_argument, NULL, 'g'},
  {"mixed-uuencode", no_argument, NULL, 'M'}, 
  {"net-headers", no_argument, NULL, 'a'}, 
  {"no-character-count", no_argument, NULL, 'w'}, 
  {"no-check-existing", no_argument, NULL, 'x'}, 
  {"no-piping", no_argument, NULL, 'P'}, 
  {"no-timestamp", no_argument, NULL, 'm'}, 
  {"output-prefix", required_argument, NULL, 'o'},
  {"query-user", no_argument, NULL, 'X'}, 
  {"quiet", no_argument, NULL, 'q'}, 
  {"quiet-unshar", no_argument, NULL, 'Q'}, 
  {"split-size-limit", required_argument, NULL, 'L'},
  {"stdin-file-list", no_argument, NULL, 'S'}, 
  {"submitter", required_argument, NULL, 's'},
  {"text-files", no_argument, NULL, 'T'}, 
  {"uuencode", no_argument, NULL, 'B'}, 
  {"vanilla-operation", no_argument, NULL, 'V'}, 
  {"whole-size-limit", required_argument, NULL, 'l'},

  {"help", no_argument, &show_help, 1},
  {"version", no_argument, &show_version, 1},

  { 0, 0, 0, 0 },
};

/*---.
| ?  |
`---*/

int
main (int argc, char *const *argv)
{
  int status = EXIT_SUCCESS;
  int stdin_file_list = 0;
  int optchar;

_setmode( _fileno( stdin ), _O_BINARY );
_setmode( _fileno( stdout ), _O_BINARY );

  program_name = argv[0];

  while (optchar = getopt_long (argc, argv,
				"$BCFL:MPQSTVXZab:cd:fg:hl:mn:o:pqs:wxz",
				long_options, NULL),
	 optchar != EOF)
    switch (optchar)
      {
      case '\0':
	break;

      case '$':
#if DEBUG
	debugging_mode = 1;
#else
	error (0, 0, "DEBUG was not selected at compile time");
#endif
	break;

      case 'B':
	set_file_mode ('B');
	break;

      case 'F':
	mandatory_prefix_mode = 1;
	break;

      case 'L':
	if (file_size_limit = atoi (optarg), file_size_limit > 1)
	  file_size_limit--;
	split_file_mode = file_size_limit != 0;
	inhibit_piping_mode = 1;
	DEBUG_PRINT ("Hard limit %dk\n", file_size_limit);
	break;

      case 'M':
	set_file_mode ('M');
	break;

      case 'P':
	inhibit_piping_mode = 1;
	break;

      case 'Q':
	quiet_unshar_mode = 1;
	break;

      case 'S':
	stdin_file_list = 1;
	break;

      case 'V':
	vanilla_operation_mode = 1;
	break;

      case 'T':
	set_file_mode ('T');
	break;

      case 'X':
	query_user_mode = 1;
	check_existing_mode = 1;
	break;

      case 'b':
	bits_per_compressed_byte = atoi (optarg);
	/* Fall through.  */

      case 'C':
      case 'Z':
	if (optchar == 'C')
	  error (0, 0, "-C is being deprecated, use -Z instead");
	set_file_mode ('Z');
	break;

      case 'a':
	net_headers_mode = 1;
	break;

      case 'c':
	cut_mark_mode = 1;
	break;

      case 'd':
	here_delimiter = optarg;
	break;

      case 'f':
	basename_mode = 1;
	break;

      case 'h':
	usage (EXIT_SUCCESS);
	break;

      case 'l':
	if (file_size_limit = atoi (optarg), file_size_limit > 1)
	  file_size_limit--;
	split_file_mode = 0;
	DEBUG_PRINT ("Soft limit %dk\n", file_size_limit);
	break;

      case 'm':
	timestamp_mode = 0;
	break;

      case 'n':
	archive_name = optarg;
	break;

      case 'o':
	strcpy (output_base_name, optarg);
	strcat (output_base_name, ".");
	part_number = 1;
	strcpy (output_filename, output_base_name);
	strcat (output_filename, "01");
	output = fopen (output_filename, "w");
	if (!output)
	  error (EXIT_FAILURE, errno, output_filename);
	break;

      case 'p':
	intermixed_parameter_mode = 1;
	break;

      case 'q':
	quiet_mode = 1;
	break;

      case 's':
	submitter_address = optarg;
	break;

      case 'w':
	character_count_mode = 0;
	break;

      case 'x':
	check_existing_mode = 0;
	break;

      case 'g':
	gzip_compression_level = atoi (optarg);
	/* Fall through.  */

      case 'z':
	set_file_mode ('z');
	break;

      default:
	usage (EXIT_FAILURE);
      }
 
  if (show_version)
    {
      printf ("GNU %s %s\n", PRODUCT, VERSION);
      exit (EXIT_SUCCESS);
    }

  if (show_help)
    usage (EXIT_SUCCESS);

  line_prefix = (here_delimiter[0] == DEFAULT_LINE_PREFIX_1
		 ? DEFAULT_LINE_PREFIX_2
		 : DEFAULT_LINE_PREFIX_1);

  here_delimiter_length = strlen (here_delimiter);

  if (vanilla_operation_mode)
    {
      if (mixed_uuencoded_file_mode < 0)
	set_file_mode ('T');

      /* Implies -m, -w, -F and -P.  */

      timestamp_mode = 0;
      character_count_mode = 0;
      mandatory_prefix_mode = 1;
      inhibit_piping_mode = 1;

      /* Forbids -X.  */

      if (query_user_mode)
	{
	  error (0, 0, "WARNING: No user interaction in vanilla mode");
	  query_user_mode = 0;
	}

      /* Diagnose if not in -T state.  */

      if (mixed_uuencoded_file_mode
	  || uuencoded_file_mode
	  || gzipped_file_mode
	  || compressed_file_mode
	  || intermixed_parameter_mode)
	error (0, 0, "WARNING: Non-text storage options overridden");
    }

  /* Set defaults for unset options.  */

  if (mixed_uuencoded_file_mode < 0)
    set_file_mode ('M');

  if (!submitter_address)
    {
      submitter_address = xmalloc (128);
      get_submitter (submitter_address);
    }

  if (!output)
    output = stdout;

  /* Maybe prepare to decide dynamically about file type.  */

  if (mixed_uuencoded_file_mode || intermixed_parameter_mode)
    {
      memset ((char *) byte_is_binary, 1, 256);
      byte_is_binary['\b'] = 0;
      byte_is_binary['\t'] = 0;
      byte_is_binary['\f'] = 0;
      memset ((char *) byte_is_binary + 32, 0, 127 - 32);
    }

  /* Maybe read file list from standard input.  */

  if (stdin_file_list)
    {
      char stdin_buf[258];
      char **list;

      argc = 0;
      list = (char **) xmalloc (1024 * sizeof (char *));
      stdin_buf[0] = 0;
      while (fgets (stdin_buf, sizeof (stdin_buf), stdin))
	{
	  if (argc == 1024)
	    error (EXIT_FAILURE, 0,
		   "Cannot handle more than 1024 files from stdin");
	  if (stdin_buf[0])
	    stdin_buf[strlen (stdin_buf) - 1] = 0;
	  list[argc] = xmalloc (strlen (stdin_buf) + 1);
	  strcpy (list[argc], stdin_buf);
	  argc++;
	  stdin_buf[0] = 0;
	}
      argv = list;
      optind = 0;
    }

  /* Diagnose various usage errors.  */

  if (optind >= argc)
    {
      error (0, 0, "No input files");
      usage (EXIT_FAILURE);
    }

  if (net_headers_mode && !archive_name)
    {
      error (0, 0, "Cannot use -a option without -n");
      usage (EXIT_FAILURE);
    }

  if (file_size_limit && !part_number)
    {
      error (0, 0, "Cannot use -l or -L option without -o");
      usage (EXIT_FAILURE);
    }

  /* Start making the archive file.  */

  generate_full_header (argc - optind, &argv[optind]);

  if (query_user_mode)
    {
      quiet_unshar_mode = 0;
      if (net_headers_mode)
	error (0, 0, "PLEASE avoid -X shars on Usenet or public networks");

      fputs ("\
shar_wish=\n",
	     output);
    }

  first_file_position = ftell (output);

  /* Process positional parameters and files.  */

  for (; optind < argc; optind++)
    if (intermixed_parameter_mode)
      if (strcmp (argv[optind], "-B") == 0)
	set_file_mode ('B');
      else if (strcmp (argv[optind], "-T") == 0)
	set_file_mode ('T');
      else if (strcmp (argv[optind], "-M") == 0)
	set_file_mode ('M');
      else if (strcmp (argv[optind], "-z") == 0)
	set_file_mode ('z');
      else if (strcmp (argv[optind], "-Z") == 0
	       || strcmp (argv[optind], "-C") == 0)
	set_file_mode ('Z');
      else
	{
	  if (walktree (shar, argv[optind]))
	    status = EXIT_FAILURE;
	}
    else
      {
	if (walktree (shar, argv[optind]))
	  status = EXIT_FAILURE;
      }

  /* Delete the sequence file, if any.  */

  if (split_file_mode && part_number > 1)
    {
      fputs ("\
rm -f _sharseq.tmp\n\
echo 'You have unpacked the last part'\n",
	     output);
      if (!quiet_mode)
	fprintf (stderr, "Created %d files\n", part_number);
    }

  fputs ("\
exit 0\n",
	 output);

  exit (status);
}

