/* idread.c -- functions to read ID database files
   Copyright (C) 1995, 1996 Free Software Foundation, Inc.
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
#include "idfile.h"
#include "xstddef.h"
#include "hash.h"
#include "error.h"
#include "xobstack.h"
#include "xmalloc.h"
#include "xnls.h"

int fgets0 __P((char *buf0, int size, FILE *in_FILE));


/****************************************************************************/

/* read_id_file opens the ID file, reads header fields into idh,
   verifies the magic number and version, and reads the constituent
   file names.  Any errors are considered fatal and cause an exit.  */

struct file_link **
read_id_file (char const *id_file_name, struct idhead *idhp)
{
  struct file_link **flinkv = maybe_read_id_file (id_file_name, idhp);
  if (flinkv)
    return flinkv;
  error (1, errno, _("can't open `%s'"), id_file_name);
  return NULL;
}

/* maybe_read_id_file does everything that read_id_file does, but is
   tolerant of errors opening the ID file, returning NULL in this case
   (this is called from mkid where an ID might or might not already
   exist).  All other errors are considered fatal.  */

struct file_link **
maybe_read_id_file (char const *id_file_name, struct idhead *idhp)
{
  obstack_init (&idhp->idh_file_link_obstack);
  idhp->idh_FILE = fopen (id_file_name, "r");
  if (idhp->idh_FILE == 0)
    return 0;

  read_idhead (idhp);
  if (idhp->idh_magic[0] != IDH_MAGIC_0 || idhp->idh_magic[1] != IDH_MAGIC_1)
    error (1, 0, _("`%s' is not an ID file! (bad magic #)"), id_file_name);
  if (idhp->idh_version != IDH_VERSION)
    error (1, 0, _("`%s' is version %d, but I only grok version %d"),
	   id_file_name, idhp->idh_version, IDH_VERSION);

  fseek (idhp->idh_FILE, idhp->idh_flinks_offset, 0);
  return deserialize_file_links (idhp);
}


/****************************************************************************/

/* Read and reconstruct a serialized file_link hierarchy.  */

struct file_link **
deserialize_file_links (struct idhead *idhp)
{
  struct file_link **flinks_0 = MALLOC (struct file_link *, idhp->idh_file_links);
  struct file_link **flinks = flinks_0;
  struct file_link **members_0 = MALLOC (struct file_link *, idhp->idh_files + 1);
  struct file_link **members = members_0;
  struct file_link *flink;
  struct file_link **slot;
  int i;

  for (i = 0; i < idhp->idh_file_links; i++)
    {
      unsigned long parent_index;
      int c;

      obstack_blank (&idhp->idh_file_link_obstack, offsetof (struct file_link, fl_name));
      if (obstack_room (&idhp->idh_file_link_obstack) >= idhp->idh_max_link)
	do
	  {
	    c = getc (idhp->idh_FILE);
	    obstack_1grow_fast (&idhp->idh_file_link_obstack, c);
	  }
	while (c);
      else
	do
	  {
	    c = getc (idhp->idh_FILE);
	    obstack_1grow (&idhp->idh_file_link_obstack, c);
	  }
	while (c);
      flink = (struct file_link *) obstack_finish (&idhp->idh_file_link_obstack);
      *flinks = flink;
      io_read (idhp->idh_FILE, &flink->fl_flags, sizeof (flink->fl_flags), IO_TYPE_INT);
      io_read (idhp->idh_FILE, &parent_index, FL_PARENT_INDEX_BYTES, IO_TYPE_INT);
      flink->fl_parent = flinks_0[parent_index];
      slot = (struct file_link **) hash_find_slot (&idhp->idh_file_link_table, flink);
      if (HASH_VACANT (*slot))
	hash_insert_at (&idhp->idh_file_link_table, flink, slot);
      else
	{
	  obstack_free (&idhp->idh_file_link_obstack, flink);
	  (*slot)->fl_flags = flink->fl_flags;
	  flink = *flinks = *slot;
	}
      flinks++;
      if (flink->fl_flags & FL_MEMBER)
	*members++ = flink;
    }
  free (flinks_0);
  *members = 0;
  return members_0;
}


/****************************************************************************/

int
read_idhead (struct idhead *idhp)
{
  return io_idhead (idhp->idh_FILE, io_read, idhp);
}

/* This is like fgets(3s), except that lines are delimited by NULs
   rather than newlines.  Also, we return the number of characters
   read rather than the address of buf0.  */

int
fgets0 (char *buf0, int size, FILE * in_FILE)
{
  char *buf;
  int c;
  char *end;

  buf = buf0;
  end = &buf[size];
  while ((c = getc (in_FILE)) > 0 && buf < end)
    *buf++ = c;
  *buf = '\0';
  return (buf - buf0);
}

int
io_read (FILE *input_FILE, void *addr, unsigned int size, int io_type)
{
  if (io_type == IO_TYPE_INT || size == 1)
    {
      switch (size)
	{
	case 4:
	  *(unsigned long *)addr = getc (input_FILE);
	  *(unsigned long *)addr += getc (input_FILE) << 010;
	  *(unsigned long *)addr += getc (input_FILE) << 020;
	  *(unsigned long *)addr += getc (input_FILE) << 030;
	  break;
	case 3:
	  *(unsigned long *)addr = getc (input_FILE);
	  *(unsigned long *)addr += getc (input_FILE) << 010;
	  *(unsigned long *)addr += getc (input_FILE) << 020;
	  break;
	case 2:
	  *(unsigned short *)addr = getc (input_FILE);
	  *(unsigned short *)addr += getc (input_FILE) << 010;
	  break;
	case 1:
	  *(unsigned char *)addr = getc (input_FILE);
	  break;
	default:
	  error (1, 0, _("unsupported size in io_read (): %d"), size);
	}
    }
  else if (io_type == IO_TYPE_STR)
    fgets0 (addr, size, input_FILE);
  else if (io_type == IO_TYPE_FIX)
    fread (addr, size, 1, input_FILE);
  else
    error (0, 0, _("unknown I/O type: %d"), io_type);
  return size;
}


/****************************************************************************/

unsigned int
token_flags (char const *buf)
{
  return *(unsigned char const *)&buf[strlen (buf) + 1];
}

#define TOK_COUNT_ADDR(buf) ((unsigned char const *)(TOK_FLAGS_ADDR (buf) + 1))
#define TOK_HITS_ADDR(buf) ((unsigned char const *)(TOK_COUNT_ADDR (buf) + 2))

unsigned short
token_count (char const *buf)
{
  unsigned char const *flags = (unsigned char const *)&buf[strlen (buf) + 1];
  unsigned char const *addr = flags + 1;
  unsigned short count = *addr;
  if (*flags & TOK_SHORT_COUNT)
    count += (*++addr << 8);
  return count;
}

unsigned char const *
token_hits_addr (char const *buf)
{
  unsigned char const *flags = (unsigned char const *)&buf[strlen (buf) + 1];
  unsigned char const *addr = flags + 2;
  if (*flags & TOK_SHORT_COUNT)
    addr++;
  return addr;
}



/****************************************************************************/

int
tree8_count_levels (unsigned int cardinality)
{
  int levels = 1;
  cardinality--;
  while (cardinality >>= 3)
    ++levels;
  return levels;
}

int
gets_past_00 (char *tok, FILE *input_FILE)
{
  int got = 0;
  int c;
  do
    {
      do
	{
	  got++;
	  c = getc (input_FILE);
	  *tok++ = c;
	}
      while (c > 0);
      got++;
      c = getc (input_FILE);
      *tok++ = c;
    }
  while (c > 0);
  return got - 2;
}

int
skip_past_00 (FILE *input_FILE)
{
  int skipped = 0;
  do
    {
      do
	skipped++;
      while (getc (input_FILE) > 0);
      skipped++;
    }
  while (getc (input_FILE) > 0);
  return skipped;
}
