/* idfile.c -- read & write mkid database file header
   Copyright (C) 1986, 1995, 1996 Free Software Foundation, Inc.
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
#include <stdio.h>
#include <errno.h>
#include "xstdlib.h"
#include "xstring.h"
#include "xsysstat.h"
#include "xnls.h"
#include "xobstack.h"
#include "idfile.h"
#include "error.h"

int io_size __P((FILE *, void *, unsigned int size, int));


/****************************************************************************/

/* Discover the name of the ID file.  If ARG is NULL, consult $IDPATH.
   If $IDPATH is undefined, default to "ID".  If the candidate file
   name is relative, search successive ancestor directories until the
   file is found or we reach the root.  If we find it, return the
   relative file name, otherwise return NULL.  */

char const *
locate_id_file_name (char const *arg)
{
  static char file_name_buffer[BUFSIZ];
  char *buf = file_name_buffer;
  char *id_path = 0;
  struct stat rootb;
  struct stat statb;

  if (arg == 0)
    {
      id_path = getenv ("IDPATH");
      if (id_path)
	{
	  id_path = strdup (id_path);
	  arg = strtok (id_path, ":");
	  /* FIXME: handle multiple ID file names */
	}
    }
  if (arg == 0)
    arg = DEFAULT_ID_FILE_NAME;

  /* if we got absolute name, just use it. */
  if (arg[0] == '/')
    return arg;
  /* if the name we were give exists, don't bother searching */
  if (stat (arg, &statb) == 0)
    return arg;
  /* search up the tree until we find a directory where this
	 * relative file name is visible.
	 * (or we run out of tree to search by hitting root).
	 */

  if (stat ("/", &rootb) != 0)
    return NULL;
  do
    {
      strcpy (buf, "../");
      buf += 3;
      strcpy (buf, arg);
      if (stat (file_name_buffer, &statb) == 0)
	return file_name_buffer;
      *buf = '\0';
      if (stat (file_name_buffer, &statb) != 0)
	return NULL;
    }
  while (statb.st_ino != rootb.st_ino || statb.st_dev != rootb.st_dev);
  return NULL;
}


/****************************************************************************/

int
sizeof_idhead ()
{
  return io_idhead (0, io_size, 0);
}

int
io_size (FILE *ignore_FILE, void *ignore_addr, unsigned int size, int io_type)
{
  if (io_type == IO_TYPE_STR)
    error (0, 0, _("can't determine the io_size of a string!"));
  return size;
}

/* The sizes of the fields must be hard-coded.  They aren't
   necessarily the sizes of the struct members, because some
   architectures don't have any way to declare 4-byte integers
   (e.g., Cray) */

int
io_idhead (FILE *fp, io_func_t iof, struct idhead *idhp)
{
  unsigned int size = 0;
  unsigned char pad = 0;
  if (fp)
    fseek (fp, 0L, 0);
  size += iof (fp, idhp->idh_magic, 2, IO_TYPE_FIX);
  size += iof (fp, &pad, 1, IO_TYPE_FIX);
  size += iof (fp, &idhp->idh_version, 1, IO_TYPE_FIX);
  size += iof (fp, &idhp->idh_flags, 2, IO_TYPE_INT);
  size += iof (fp, &idhp->idh_file_links, 4, IO_TYPE_INT);
  size += iof (fp, &idhp->idh_files, 4, IO_TYPE_INT);
  size += iof (fp, &idhp->idh_tokens, 4, IO_TYPE_INT);
  size += iof (fp, &idhp->idh_buf_size, 4, IO_TYPE_INT);
  size += iof (fp, &idhp->idh_vec_size, 4, IO_TYPE_INT);
  size += iof (fp, &idhp->idh_tokens_offset, 4, IO_TYPE_INT);
  size += iof (fp, &idhp->idh_flinks_offset, 4, IO_TYPE_INT);
  size += iof (fp, &idhp->idh_end_offset, 4, IO_TYPE_INT);
  size += iof (fp, &idhp->idh_max_link, 2, IO_TYPE_INT);
  size += iof (fp, &idhp->idh_max_path, 2, IO_TYPE_INT);
  return size;
}
