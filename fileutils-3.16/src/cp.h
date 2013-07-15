/* cp.h  -- file copying (data definitions)
   Copyright (C) 89, 90, 91, 1995 Free Software Foundation.

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
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   Written by Torbjorn Granlund, Sweden (tege@sics.se). */

#include <sys/types.h>
#include "system.h"
#include "error.h"

struct dir_list
{
  struct dir_list *parent;
  ino_t ino;
  dev_t dev;
};

struct entry
{
  ino_t ino;
  dev_t dev;
  char *node;			/* Path name, or &new_file for new inodes.  */
  struct entry *coll_link;	/* 0 = entry not occupied.  */
};

struct htab
{
  unsigned modulus;		/* Size of the `hash' pointer vector.  */
  struct entry *entry_tab;	/* Pointer to dynamically growing vector.  */
  unsigned entry_tab_size;	/* Size of current `entry_tab' allocation.  */
  unsigned first_free_entry;	/* Index in `entry_tab'.  */
  struct entry *hash[1];	/* Vector of pointers in `entry_tab'.  */
};

/* For created inodes, a pointer in the search structure to this
   character identifies the inode as being new.  */
extern char new_file;

char *basename ();
char *xmalloc ();
char *xrealloc ();
char *stpcpy ();
char *savedir ();
int yesno ();
void hash_init ();
void strip_trailing_slashes ();

void forget_all __P ((void));
void usage __P ((int status));
char *hash_insert __P ((ino_t ino, dev_t dev, const char *node));
char *hash_insert2 __P ((struct htab *htab, ino_t ino, dev_t dev,
			 const char *node));
char *remember_copied __P ((const char *node, ino_t ino, dev_t dev));
int remember_created __P ((const char *path));

#ifndef S_IWRITE
#define S_IWRITE S_IWUSR
#endif
