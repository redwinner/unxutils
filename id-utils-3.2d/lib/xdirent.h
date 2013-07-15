/* xdirent.h -- dirent declarations wrapper
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

#ifndef _xdirent_h_
#define _xdirent_h_

#if HAVE_DIRENT_H
# include <dirent.h>
#endif
#if HAVE_NDIR_H
# include <ndir.h>
#endif
#if HAVE_SYS_DIR_H
# include <sys/dir.h>
#endif
#if HAVE_SYS_NDIR_H
# include <sys/ndir.h>
#endif

/* Interpret `HAVE_LINK' as meaning `UN*X style' directory structure
   (e.g., A single root called `/', with `/' separating links), and
   !HAVE_LINK as `DOS|OS/2|Windows style' (e.g., Multiple root volues
   named `x:', with `\' separating links).  */

#if HAVE_LINK
# define IS_ABSOLUTE(_dir_) ((_dir_)[0] == '/')
# define SLASH_STRING "/"
# define SLASH_CHAR '/'
# define DOT_DOT_SLASH "../"
#else
/* NEEDSWORK: prefer forward-slashes as a user-configurable option.  */
# define IS_ABSOLUTE(_dir_) ((_dir_)[1] == ':')
# define SLASH_STRING "\\/"
# define SLASH_CHAR '\\'
# define DOT_DOT_SLASH "..\\"
#endif

#endif /* not _xdirent_h_ */
