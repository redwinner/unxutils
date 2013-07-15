/* mswindows.c -- Windows-specific support
   Copyright (C) 1995, 1996, 1997, 1998  Free Software Foundation, Inc.

This file is part of Wget.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* #### Someone document these functions!  */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <string.h>
#include <assert.h>

//#include "wget.h"
//#include "url.h"

char *argv0;



/* Emulation of Unix sleep.  */
unsigned int
sleep (unsigned seconds)
{
  Sleep (1000 * seconds);
  /* Unix sleep() is interruptible.  To make it semi-usable, it
     returns a value that says how much it "really" slept, or some
     junk like that.  Ignore it.  */
  return 0U;
}

static char *
read_registry (HKEY hkey, char *subkey, char *valuename, char *buf, int *len)
{
  HKEY result;
  DWORD size = *len;
  DWORD type = REG_SZ;
  if (RegOpenKeyEx (hkey, subkey, NULL, KEY_READ, &result) != ERROR_SUCCESS)
    return NULL;
  if (RegQueryValueEx (result, valuename, NULL, &type, buf, &size) != ERROR_SUCCESS)
    buf = NULL;
  *len = size;
  RegCloseKey (result);
  return buf;
}

char *
pwd_cuserid (char *where)
{
  char buf[32], *ptr;
  int len = sizeof (buf);
  if (GetUserName (buf, (LPDWORD) &len) == TRUE)
    {
      ;
    }
  else if (!!(ptr = getenv ("USERNAME")))
    {
      strcpy (buf, ptr);
    }
  else if (!read_registry (HKEY_LOCAL_MACHINE, "Network\\Logon",
			  "username", buf, &len))
    {
      return NULL;
    }
  if (where)
    {
      strncpy (where, buf, len);
      return where;
    }
  return strdup (buf);
}

/* Winsock stuff. */

static void
ws_cleanup (void)
{
  WSACleanup ();
}

#if 0
static void
ws_hangup (void)
{
  redirect_output (_("\n\
CTRL+Break received, redirecting output to `%s'.\n\
Execution continued in background.\n\
You may stop Wget by pressing CTRL+ALT+DELETE.\n"));
}


#define    ws_backgnd fork_to_background

void
fork_to_background (void)
{
  /* Whether we arrange our own version of opt.lfilename here.  */
  int changedp = 0;

  if (!opt.lfilename)
    {
      opt.lfilename = unique_name (DEFAULT_LOGFILE);
      changedp = 1;
    }
  printf (_("Continuing in background.\n"));
  if (changedp)
    printf (_("Output will be written to `%s'.\n"), opt.lfilename);

  ws_hangup ();
  if (!windows_nt_p)
    FreeConsole ();
}
#endif

static BOOL WINAPI
ws_handler (DWORD dwEvent)
{
  switch (dwEvent)
    {
#ifdef CTRLC_BACKGND
    case CTRL_C_EVENT:
    ws_backgnd ();
    break;
#endif
#ifdef CTRLBREAK_BACKGND
    case CTRL_BREAK_EVENT:
    ws_backgnd ();
    break;
#endif

    case CTRL_SHUTDOWN_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
    default:
      WSACleanup ();
      return FALSE;
    }
  return TRUE;
}

#if 0
void
ws_changetitle (char *url, int nurl)
{
  char *title_buf;
  if (!nurl)
    return;

  title_buf = (char *)xmalloc (strlen (url) + 20);
  sprintf (title_buf, "Wget %s%s", url, nurl == 1 ? "" : " ...");
  /* #### What are the semantics of SetConsoleTitle?  Will it free the
     given memory later?  */
  SetConsoleTitle (title_buf);
}

char *
ws_mypath (void)
{
  static char *wspathsave;
  char *buffer;
  int rrr;
  char *ptr;

  if (wspathsave)
    {
      return wspathsave;
    }
  ptr = strrchr (argv0, '\\');
  if (ptr)
    {
      *(ptr + 1) = '\0';
      wspathsave = (char*) xmalloc (strlen(argv0)+1);
      strcpy (wspathsave, argv0);
      return wspathsave;
    }
  buffer = (char*) xmalloc (256);
  rrr = SearchPath (NULL, argv0, strchr (argv0, '.') ? NULL : ".EXE",
		    256, buffer, &ptr);
  if (rrr && rrr <= 256)
    {
      *ptr = '\0';
      wspathsave = (char*) xmalloc (strlen(buffer)+1);
      strcpy (wspathsave, buffer);
      return wspathsave;
    }
  free (buffer);
  return NULL;
}


void
ws_help (const char *name)
{
  char *mypath = ws_mypath ();

  if (mypath)
    {
      struct stat sbuf;
      char *buf = (char *)alloca (strlen (mypath) + strlen (name) + 4 + 1);
      sprintf (buf, "%s%s.HLP", mypath, name);
      if (stat (buf, &sbuf) == 0) 
	{
          printf (_("Starting WinHelp %s\n"), buf);
          WinHelp (NULL, buf, HELP_INDEX, NULL);
        }
      else
        {
          printf ("%s: %s\n", buf, strerror (errno));
        }
    }
}
#endif

void
ws_startup (void)
{
  WORD requested;
  WSADATA data;
  int err;
  OSVERSIONINFO os;

  GetVersionEx (&os);

  requested = MAKEWORD (1, 1);
  err = WSAStartup (requested, &data);

  if (err != 0)
    {
      fprintf (stderr, "Couldn't find usable socket driver.\n");
      exit (1);
    }

  if (LOBYTE (requested) < 1 || (LOBYTE (requested) == 1 &&
				HIBYTE (requested) < 1))
    {
      fprintf (stderr, "Couldn't find usable socket driver.\n");
      WSACleanup ();
      exit (1);
    }
  atexit (ws_cleanup);
  SetConsoleCtrlHandler (ws_handler, TRUE);
}
