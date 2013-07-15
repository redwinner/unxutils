/* xmalloc.h -- malloc declarations wrapper
   Copyright (C) 1996 Free Software Foundation, Inc.

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

#ifndef _xmalloc_h_
#define _xmalloc_h_ 1

#include <sys/types.h>

#if HAVE_CONFIG_H
# include <config.h>
#endif
#if defined STDC_HEADERS || defined _LIBC || defined HAVE_STDLIB_H
# include <stdlib.h>
#endif
#if HAVE_MALLOC_H
# include <malloc.h>
#else
void free ();
#endif

void *xmalloc __P((size_t n));
void *xrealloc __P((void *p, size_t n));

#include "xstring.h"

#define	CALLOC(t, n) ((t *) calloc (sizeof (t), (n)))
#define MALLOC(t, n) ((t *) xmalloc (sizeof (t) * (n)))
#define REALLOC(o, t, n) ((t *) xrealloc ((o), sizeof (t) * (n)))
#define CLONE(o, t, n) ((t *) memcpy (MALLOC (t, (n)), (o), sizeof (t) * (n)))

#endif /* _xmalloc_h_ */
