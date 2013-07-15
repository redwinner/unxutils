/* dynvec.h -- declarations for dynamically growable vectors
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

#ifndef _dynvec_h_
#define _dynvec_h_

struct dynvec
{
  void **dv_vec;
  int dv_capacity;
  int dv_fill;
};

struct dynvec *make_dynvec __P((int n));
void dynvec_free __P((struct dynvec *dv));
void dynvec_freeze __P((struct dynvec *dv));
void dynvec_append __P((struct dynvec *dv, void *element));

#endif /* not _dynvec_h_ */
