/* xobstack.h -- obstack declarations wrapper
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

#ifndef _xobstack_h_
#define _xobstack_h_

#include <obstack.h>
#include "xmalloc.h"

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free
#define OBSTACK_ALLOC(obs, t, n) ((t *)obstack_alloc ((obs), (n)*sizeof(t)))

#endif /* not _xobstack_h_ */
