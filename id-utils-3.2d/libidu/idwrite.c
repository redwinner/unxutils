/* idwrite.c -- functions to write ID database files
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
#include "idfile.h"
#include "hash.h"
#include "xobstack.h"
#include "xmalloc.h"
#include "xnls.h"
#include "error.h"

int file_link_qsort_compare __P((void const *x, void const *y));


/****************************************************************************/
/* Serialize and write a file_link hierarchy. */

void
serialize_file_links (struct idhead *idhp)
{
  struct file_link **flinks_0;
  struct file_link **flinks;
  struct file_link **end;
  struct file_link **parents_0;
  struct file_link **parents;
  unsigned long parent_index = 0;
  int max_link = 0;

  flinks_0 = (struct file_link **) hash_dump (&idhp->idh_file_link_table,
					      0, file_link_qsort_compare);
  end = &flinks_0[idhp->idh_file_link_table.ht_fill];
  parents = parents_0 = MALLOC (struct file_link *, idhp->idh_file_link_table.ht_fill);
  for (flinks = flinks_0; flinks < end; flinks++)
    {
      struct file_link *flink = *flinks;
      int name_length;

      if (!(flink->fl_flags & FL_USED))
	break;
      name_length = strlen (flink->fl_name);
      if (name_length > max_link)
	max_link = name_length;
      io_write (idhp->idh_FILE, flink->fl_name, 0, IO_TYPE_STR);
      io_write (idhp->idh_FILE, &flink->fl_flags, sizeof (flink->fl_flags), IO_TYPE_INT);
      io_write (idhp->idh_FILE, (IS_ROOT_FILE_LINK (flink)
				? &parent_index : &flink->fl_parent->fl_index),
		FL_PARENT_INDEX_BYTES, IO_TYPE_INT);
      *parents++ = flink->fl_parent; /* save parent link before clobbering */
      flink->fl_index = parent_index++;
    }
  /* restore parent links */
  for ((flinks = flinks_0), (parents = parents_0); flinks < end; flinks++)
    {
      struct file_link *flink = *flinks;
      if (!(flink->fl_flags & FL_USED))
	break;
      flink->fl_parent = *parents++;
    }
  free (parents_0);
  free (flinks_0);
  idhp->idh_max_link = max_link + 1;
  idhp->idh_file_links = parent_index;
  idhp->idh_files = idhp->idh_member_file_table.ht_fill;
}

/* Collation sequence:
   - Used before unused.
   - Among used: breadth-first (dirs before files, parent dirs before children)
   - Among files: collate by mf_index.  */

int
file_link_qsort_compare (void const *x, void const *y)
{
  struct file_link const *flx = *(struct file_link const *const *) x;
  struct file_link const *fly = *(struct file_link const *const *) y;
  unsigned int x_flags = flx->fl_flags;
  unsigned int y_flags = fly->fl_flags;
  int result;

  result = (y_flags & FL_USED) - (x_flags & FL_USED);
  if (result)
    return result;
  if (!(x_flags & FL_USED))	/* If neither link is used, we don't care... */
    return 0;
  result = (y_flags & FL_TYPE_DIR) - (x_flags & FL_TYPE_DIR);
  if (result)
    return result;
  result = (y_flags & FL_TYPE_MASK) - (x_flags & FL_TYPE_MASK);
  if (result)
    return result;
  if (FL_IS_FILE (x_flags))
    {
      struct member_file *x_member = find_member_file (flx);
      struct member_file *y_member = find_member_file (fly);
      return x_member->mf_index - y_member->mf_index;
    }
  else
    {
      int x_depth = links_depth (flx);
      int y_depth = links_depth (fly);
      return (x_depth - y_depth);
    }
}


/****************************************************************************/

int
write_idhead (struct idhead *idhp)
{
  return io_idhead (idhp->idh_FILE, io_write, idhp);
}

int
io_write (FILE *output_FILE, void *addr, unsigned int size, int io_type)
{
  if (io_type == IO_TYPE_INT || size == 1)
    {
      switch (size)
	{
	case 4:
	  putc (*(unsigned long *)addr, output_FILE);
	  putc (*(unsigned long *)addr >> 010, output_FILE);
	  putc (*(unsigned long *)addr >> 020, output_FILE);
	  putc (*(unsigned long *)addr >> 030, output_FILE);
	  break;
	case 3:
	  putc (*(unsigned long *)addr, output_FILE);
	  putc (*(unsigned long *)addr >> 010, output_FILE);
	  putc (*(unsigned long *)addr >> 020, output_FILE);
	  break;
	case 2:
	  putc (*(unsigned short *)addr, output_FILE);
	  putc (*(unsigned short *)addr >> 010, output_FILE);
	  break;
	case 1:
	  putc (*(unsigned char *)addr, output_FILE);
	  break;
	default:
	  error (1, 0, _("unsupported size in io_write (): %d"), size);
	}
    }
  else if (io_type == IO_TYPE_STR)
    {
      fputs (addr, output_FILE);
      putc ('\0', output_FILE);
    }
  else if (io_type == IO_TYPE_FIX)
    fwrite (addr, size, 1, output_FILE);
  else
    error (0, 0, _("unknown I/O type: %d"), io_type);
  return size;
}
