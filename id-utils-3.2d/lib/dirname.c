/* basename.c -- return the leading elements in a path
   Copyright (C) 1996 Free Software Foundation, Inc.

The Linux C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The Linux C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <ansidecl.h>
#include "xstring.h"

/* Return NAME with any leading path stripped off.  */

char *
DEFUN(dirname, (name), CONST char *name)
{
  char *base;

  base = strrchr (name, '/');
  if (base)
    {
      while (*--base == '/')
	;
      base++;
    }
  return base ? strndup (name, base - name) : ".";
}
