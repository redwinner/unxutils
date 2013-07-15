/* xfnmatch.h -- fnmatch declarations wrapper
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

#ifndef _xfnmatch_h_
#define _xfnmatch_h_

#include <fnmatch.h>

#ifndef FNM_FILE_NAME
# define FNM_FILE_NAME FNM_PATHNAME
#endif

#if HAVE_LINK
# define MAYBE_FNM_CASEFOLD 0
#else
# define MAYBE_FNM_CASEFOLD FNM_CASEFOLD
#endif

#endif /* not _xfnmatch_h_ */
