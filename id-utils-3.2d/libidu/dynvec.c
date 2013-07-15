/* dynvec.c -- dynamically growable vectors
   Copyright (C) 1995 Free Software Foundation, Inc.
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
#include "dynvec.h"
#include "xmalloc.h"

struct dynvec *
make_dynvec (int n)
{
  struct dynvec *dv = MALLOC (struct dynvec, 1);
  dv->dv_vec = MALLOC (void *, n);
  dv->dv_capacity = n;
  dv->dv_fill = 0;
  return dv;
}

void
dynvec_free (struct dynvec *dv)
{
  free (dv->dv_vec);
  free (dv);
}

void
dynvec_freeze (struct dynvec *dv)
{
  if (dv->dv_fill == dv->dv_capacity)
    return;
  dv->dv_capacity = dv->dv_fill;
  dv->dv_vec = REALLOC (dv->dv_vec, void *, dv->dv_capacity);
}

void
dynvec_append (struct dynvec *dv, void *element)
{
  if (dv->dv_fill == dv->dv_capacity)
    {
      dv->dv_capacity *= 2;
      dv->dv_vec = REALLOC (dv->dv_vec, void *, dv->dv_capacity);
    }
  dv->dv_vec[dv->dv_fill++] = element;
}
