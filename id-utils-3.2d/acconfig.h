/* Special definitions for GNU id-utils, processed by autoheader.
   Copyright (C) 1995, 96, 99 Free Software Foundation, Inc.
*/

#ifndef _config_h_
#define _config_h_

@TOP@

/* Define if your sys/types.h file defines the type ptrdiff_t.  */
#undef HAVE_PTRDIFF_T

/* Define if you have the <sys/ioctl.h> header file.  */
#undef HAVE_SYS_IOCTL_H

/* Define to the name of the installed language map file.  */
#undef LANGUAGE_MAP_FILE


@BOTTOM@

/* According to Thomas Neumann, NeXT POSIX termios support is losing,
   and sgtty is the way to go.  Note: the comment between #undef &
   HAVE_TERMIOS_H is necessary to defeat configure's edits.  */

#if HAVE_SGTTY_H
# ifdef NeXT
#  undef /**/ HAVE_TERMIOS_H
# endif
#endif

#ifndef __P
# ifndef PROTOTYPES
#  define	__P(protos)	()		/* traditional C */
# else
#  define	__P(protos)	protos		/* full-blown ANSI C */
# endif
#endif

#endif /* not _config_h_ */
