/* xalloca.h -- alloca declarations wrapper
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

#ifndef _xalloca_h_
#define _xalloca_h_ 1

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef alloca
# ifdef __GNUC__
#  define alloca __builtin_alloca
#  define HAVE_ALLOCA 1
# else
#  if defined HAVE_ALLOCA_H || defined _LIBC
#   include <alloca.h>
#  else
#   ifdef _AIX
 #pragma alloca
#   else
#    ifndef alloca
char *alloca ();
#    endif
#   endif
#  endif
# endif
#endif

#define ALLOCA(t, n) ((t *) alloca (sizeof (t) * (n)))

#endif /* _xalloca_h_ */
