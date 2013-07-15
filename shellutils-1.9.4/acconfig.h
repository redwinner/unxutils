/* acconfig.h
   This file is in the public domain.

   Descriptive text for the C preprocessor macros that
   the distributed Autoconf macros can define.
   No software package will use all of them; autoheader copies the ones
   your configure.in uses into your configuration header file templates.

   The entries are in sort -df order: alphabetical, case insensitive,
   ignoring punctuation (such as underscores).  Although this order
   can split up related entries, it makes it easier to check whether
   a given entry is in the file.

   Leave the following blank line there!!  Autoheader needs it.  */


/* Define if your system defines TIOCGWINSZ in sys/ioctl.h.  */
#undef GWINSZ_IN_SYS_IOCTL

/* Define if your system's definition of `struct termios' has a member
   named c_line.  */
#undef HAVE_C_LINE

/* Define if your system has SysV shadow passwords and the shadow.h header.  */
#undef HAVE_SHADOW_H

/* Define if your system lacks gethostname but has the uname system call.  */
#undef HAVE_UNAME

/* Define if your system's struct utmp has a member named ut_host.  */
#undef HAVE_UT_HOST

/* Define if you have the <utmpx.h> header file.  */
#undef HAVE_UTMPX_H

/* Define if your system lacks the getpriority and setpriority system calls,
   but has `nice' instead.  */
#undef NICE_PRIORITY

/* Define if your system defines `struct winsize' in sys/ptem.h.  */
#undef WINSIZE_IN_PTEM


/* Leave that blank line there!!  Autoheader needs it.
   If you're adding to this file, keep in mind:
   The entries are in sort -df order: alphabetical, case insensitive,
   ignoring punctuation (such as underscores).  */
