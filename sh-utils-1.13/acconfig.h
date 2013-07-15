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


/* Define if using getloadavg.c.  */
#undef C_GETLOADAVG

/* Define if your system defines TIOCGWINSZ in sys/pty.h.  */
#undef GWINSZ_IN_SYS_PTY

/* Define if your system defines TIOCGWINSZ in sys/ioctl.h.  */
#undef GWINSZ_IN_SYS_IOCTL

/* Define to 1 if NLS is requested.  */
#undef ENABLE_NLS

/* Define as 1 if you have catgets and don't want to use GNU gettext.  */
#undef HAVE_CATGETS

/* Define if your system's definition of `struct termios' has a member
   named c_line.  */
#undef HAVE_C_LINE

/* Define as 1 if you have gettext and don't want to use GNU gettext.  */
#undef HAVE_GETTEXT

/* Define if your locale.h file contains LC_MESSAGES.  */
#undef HAVE_LC_MESSAGES

/* Define if your system has the /proc/uptime special file.  */
#undef HAVE_PROC_UPTIME

/* Define if your system has SysV shadow passwords and the shadow.h header.  */
#undef HAVE_SHADOW_H

/* Define to 1 if you have the stpcpy function.  */
#undef HAVE_STPCPY

/* Define if you have the syslog function.  */
#undef HAVE_SYSLOG

/* Define if your system's struct utmp has a member named ut_host.  */
#undef HAVE_UT_HOST

/* Define if you have the <utmpx.h> header file.  */
#undef HAVE_UTMPX_H

/* Define if localtime caches TZ, despite what Posix requires.  */
#undef LOCALTIME_CACHE

/* Define to gnu_mktime if the replacement function should be used.  */
#undef mktime

/* Define if your system lacks the getpriority and setpriority system calls,
   but has `nice' instead.  */
#undef NICE_PRIORITY

/* Define to the name of the distribution.  */
#undef PACKAGE

/* The concatenation of the strings "GNU ", and PACKAGE.  */
#undef GNU_PACKAGE

/* Define to 1 if ANSI function prototypes are usable.  */
#undef PROTOTYPES

/* Define to gnu_strftime if the replacement function should be used.  */
#undef strftime

/* Define if you need _XOPEN_SOURCE in order to make termios.h define
   all of the useful symbols.  */
#undef TERMIOS_NEEDS_XOPEN_SOURCE

/* Define to the version of the distribution.  */
#undef VERSION

/* Define if your system defines `struct winsize' in sys/ptem.h.  */
#undef WINSIZE_IN_PTEM


/* Leave that blank line there!!  Autoheader needs it.
   If you're adding to this file, keep in mind:
   The entries are in sort -df order: alphabetical, case insensitive,
   ignoring punctuation (such as underscores).  */
