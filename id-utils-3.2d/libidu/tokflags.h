/* tokflags.h -- decls for per-token flags
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

#ifndef _tokflags_h_
#define _tokflags_h_

#define TOK_VECTOR	0x01	/* 1 = hits are stored as a vector
				   0 = hits are stored as a 8-way tree of bits
				   mkid chooses whichever is more compact.
				   vector is more compact for tokens with few hits */
#define TOK_NUMBER	0x02	/* occurs as a number */
#define TOK_NAME	0x04	/* occurs as a name */
#define TOK_STRING	0x08	/* occurs in a string */
#define TOK_LITERAL	0x10	/* occurs as a literal */
#define TOK_COMMENT	0x20	/* occurs in a comment */
#define TOK_UNUSED_1	0x40
#define TOK_SHORT_COUNT	0x80	/* count is two bytes */

#endif /* not _tokflags_h_ */
