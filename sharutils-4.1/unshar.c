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

/* Unpackage one or more shell archive files.  The `unshar' program is a
   filter which removes the front part of a file and passes the rest to
   the `sh' command.  It understands phrases like "cut here", and also
   knows about shell comment characters and the Unix commands `echo',
   `cat', and `sed'.  */

#include "system.h"
#include "getopt.h"

/* Buffer size for holding a file name.  */
#define NAME_BUFFER_SIZE 1024

/* Buffer size for shell process input.  */
#define SHELL_BUFFER_SIZE 8196

#define EOL '\n'

/* The name this program was run with. */
const char *program_name;

/* If non-zero, display usage information and exit.  */
static int show_help = 0;

/* If non-zero, print the version on standard output and exit.  */
static int show_version = 0;

static int pass_c_flag = 0;
static int continue_reading = 0;
static const char *exit_string = "exit 0";
static size_t exit_string_length;
static char *current_directory;

/*-------------------------------------------------------------------------.
| Match the leftmost part of a string.  Returns 1 if initial characters of |
| DATA match PATTERN exactly; else 0.					   |
`-------------------------------------------------------------------------*/

static int 
starting_with (const char *data, const char *pattern)
{
  const char *pattern_cursor, *data_cursor;

  pattern_cursor = pattern;
  data_cursor = data;
  do
    {
      if (*pattern_cursor == '\0')
	return 1;
    }
  while (*pattern_cursor++ == *data_cursor++);

  return 0;
}

/*-------------------------------------------------------------------------.
| For a DATA string and a PATTERN containing one or more embedded	   |
| asterisks (matching any number of characters), return non-zero if the	   |
| match succeeds, and set RESULT_ARRAY[I] to the characters matched by the |
| I'th *.								   |
`-------------------------------------------------------------------------*/

static int
matched_by (const char *data, const char *pattern, char **result_array)
{
  const char *pattern_cursor = NULL;
  const char *data_cursor = NULL;
  char *result_cursor = NULL;
  int number_of_results = 0;

  while (1)
    if (*pattern == '*')
      {
	pattern_cursor = ++pattern;
	data_cursor = data;
	result_cursor = result_array[number_of_results++];
	*result_cursor = '\0';
      }
    else if (*data == *pattern)
      {
	if (*pattern == '\0')
	  /* The pattern matches.  */
	  return 1;

	pattern++;
	data++;
      }
    else
      {
	if (*data == '\0')
	  /* The pattern fails: no more data.  */
	  return 0;

	if (pattern_cursor == NULL)
	  /* The pattern fails: no star to adjust.  */
	  return 0;

	/* Restart pattern after star.  */

	pattern = pattern_cursor;
	*result_cursor++ = *data_cursor;
	*result_cursor = '\0';

	/* Rescan after copied char.  */

	data = ++data_cursor;
      }
}

/*------------------------------------------------------------------------.
| Associated with a given file NAME, position FILE at the start of the	  |
| shell command portion of a shell archive file.  Scan file from position |
| START.								  |
`------------------------------------------------------------------------*/

static int
find_archive (const char *name, FILE *file, long start)
{
  char buffer[BUFSIZ];
  long position;

  /* Results from star matcher.  */

  static char res1[BUFSIZ], res2[BUFSIZ], res3[BUFSIZ], res4[BUFSIZ];
  static char *result[] = {res1, res2, res3, res4};

  fseek (file, start, 0);

  while (1)
    {

      /* Record position of the start of this line.  */

      position = ftell (file);

      /* Read next line, fail if no more and no previous process.  */

      if (!fgets (buffer, BUFSIZ, file))
	{
	  if (!start)
	    error (0, 0, "Found no shell commands in %s", name);
	  return 0;
	}

      /* Bail out if we see C preprocessor commands or C comments.  */

      if (starting_with (buffer, "#include")
	  || starting_with (buffer, "# include")
	  || starting_with (buffer, "#define")
	  || starting_with (buffer, "# define")
	  || starting_with (buffer, "#ifdef")
	  || starting_with (buffer, "# ifdef")
	  || starting_with (buffer, "#ifndef")
	  || starting_with (buffer, "# ifndef")
	  || starting_with (buffer, "/*"))
	{
	  error (0, 0, "%s looks like raw C code, not a shell archive", name);
	  return 0;
	}

      /* Does this line start with a shell command or comment.  */

      if (starting_with (buffer, "#")
	  || starting_with (buffer, ":")
	  || starting_with (buffer, "echo ")
	  || starting_with (buffer, "sed ")
	  || starting_with (buffer, "cat ")
	  || starting_with (buffer, "if "))
	{
	  fseek (file, position, 0);
	  return 1;
	}

      /* Does this line say "Cut here".  */

      if (matched_by (buffer, "*CUT*HERE*", result) ||
	  matched_by (buffer, "*cut*here*", result) ||
	  matched_by (buffer, "*TEAR*HERE*", result) ||
	  matched_by (buffer, "*tear*here*", result) ||
	  matched_by (buffer, "*CUT*CUT*", result) ||
	  matched_by (buffer, "*cut*cut*", result))
	{

	  /* Read next line after "cut here", skipping blank lines.  */

	  while (1)
	    {
	      position = ftell (file);

	      if (!fgets (buffer, BUFSIZ, file))
		{
		  error (0, 0, "Found no shell commands after 'cut' in %s",
			 name);
		  return 0;
		}

	      if (*buffer != '\n')
		break;
	    }

	  /* Win if line starts with a comment character of lower case
	     letter.  */

	  if (*buffer == '#' || *buffer == ':'
	      || (('a' <= *buffer) && ('z' >= *buffer)))
	    {
	      fseek (file, position, 0);
	      return 1;
	    }

	  /* Cut here message lied to us.  */

	  error (0, 0, "%s is probably not a shell archive", name);
	  error (0, 0, "the 'cut' line was followed by: %s", buffer);
	  return 0;
	}
    }
}

/*-----------------------------------------------------------------.
| Unarchive a shar file provided on file NAME.  The file itself is |
| provided on the already opened FILE.				   |
`-----------------------------------------------------------------*/

static void
unarchive_shar_file (const char *name, FILE *file)
{
  char buffer[SHELL_BUFFER_SIZE];
  int character;
  FILE *shell_process;
  long current_position = 0;
  char *more_to_read;

  while (find_archive (name, file, current_position))
    {
      printf ("%s:\n", name);
      shell_process = popen (pass_c_flag ? "sh -s - -c" : "sh", "w");
      if (!shell_process)
	error (EXIT_FAILURE, errno, "Starting `sh' process");

      if (!continue_reading)
	{
	  while (character = fgetc (file), character != EOF)
	    fputc (character, shell_process);
	  pclose (shell_process);
	  break;
	}
      else
	{
	  while (more_to_read = fgets (buffer, SHELL_BUFFER_SIZE, file),
		 more_to_read != 0)
	    {
	      fputs (buffer, shell_process);
	      if (!strncmp (exit_string, buffer, exit_string_length))
		break;
	    }
	  pclose (shell_process);

	  if (more_to_read)
	    current_position = ftell (file);
	  else
	    break;
	}
    }
}

/*-----------------------------.
| Explain how to use program.  |
`-----------------------------*/

static void
usage (int status)
{
  if (status != EXIT_SUCCESS)
    fprintf (stderr, "Try `%s --help' for more information.\n", program_name);
  else
    {
      printf ("Usage: %s [OPTION]... [FILE]...\n", program_name);
      fputs ("\
Mandatory arguments to long options are mandatory for short options too.\n\
\n\
  -d, --directory=DIRECTORY   change to DIRECTORY before unpacking\n\
  -c, --overwrite             pass -c flag to shar for overwriting files\n\
  -e, --exit-0                same as `--split-at=\"exit 0\"'\n\
  -E, --split-at=STRING       split concatenated shars after STRING\n\
      --help                  display this help and exit\n\
      --version               output version information and exit\n\
\n\
If no FILE, standard input is read.\n",
	     stdout);
    }
  exit (status);
}

/*--------------------------------------.
| Decode options and launch execution.  |
`--------------------------------------*/

static const struct option long_options[] =
{
  {"directory", required_argument, NULL, 'd'},
  {"exit-0", no_argument, NULL, 'e'}, 
  {"overwrite", no_argument, NULL, 'c'}, 
  {"split-at", required_argument, NULL, 'E'},

  {"help", no_argument, &show_help, 1},
  {"version", no_argument, &show_version, 1},

  { 0, 0, 0, 0 },
};

int
main (int argc, char *const *argv)
{
  size_t size_read;
  FILE *file;
  char name_buffer[NAME_BUFFER_SIZE];
  char copy_buffer[NAME_BUFFER_SIZE];
  int optchar;

_setmode( _fileno( stdin ), _O_BINARY );
_setmode( _fileno( stdout ), _O_BINARY );

  program_name = argv[0];

  setbuf (stdout, NULL);
  setbuf (stderr, NULL);

  if (current_directory = xgetcwd (), !current_directory)
    error (EXIT_FAILURE, errno, "Cannot get current directory name");

  /* Process options.  */

  while (optchar = getopt_long (argc, argv, "E:cd:e", long_options, NULL),
	 optchar != EOF)
    switch (optchar)
      {
      case '\0':
	break;

      case 'c':
	pass_c_flag = 1;
	break;

      case 'd':
	if (chdir (optarg) == -1)
	  error (2, 0, "Cannot chdir to `%s'", optarg);
	break;

      case 'E':
	exit_string = optarg;
	/* Fall through.  */

      case 'e':
	continue_reading = 1;
	exit_string_length = strlen (exit_string);
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

  if (optind < argc)
    for (; optind < argc; optind++)
      {
	if (argv[optind][0] == '/')
	  strcpy (name_buffer, argv[optind]);
	else
	  {
	    strcpy (name_buffer, current_directory);
	    strcat (name_buffer, "/");
	    strcat (name_buffer, argv[optind]);
	  }
	if (file = fopen (name_buffer, "r"), !file)
	  error (EXIT_FAILURE, errno, name_buffer);
	unarchive_shar_file (name_buffer, file);
	fclose (file);
      }
  else
    {
      sprintf (name_buffer, "/tmp/unsh.%05d", (int) getpid ());
      unlink (name_buffer);

      if (file = fopen (name_buffer, "w+"), !file)
	error (EXIT_FAILURE, errno, name_buffer);
#ifndef __MSDOS__
      unlink (name_buffer);	/* will be deleted on fclose */
#endif

      while (size_read = fread (copy_buffer, 1, sizeof (copy_buffer), stdin),
	     size_read != 0)
	fwrite (copy_buffer, size_read, 1, file);
      rewind (file);

      unarchive_shar_file ("standard input", file);

      fclose (file);
#ifdef __MSDOS__
      unlink (name_buffer);
#endif
    }

  exit (EXIT_SUCCESS);
}
