/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
/* #undef _ALL_SOURCE */
#endif

/* Define if using alloca.c.  */
/* #undef C_ALLOCA */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define to one of _getb67, GETB67, getb67 for Cray-2 and Cray-YMP systems.
   This function is required for alloca.c support on those systems.  */
/* #undef CRAY_STACKSEG_END */

/* Define to the type of elements in the array set by `getgroups'.
   Usually this is either `int' or `gid_t'.  */
#define GETGROUPS_T int

/* Define to `int' if <sys/types.h> doesn't define.  */
#define gid_t int

/* Define if you have alloca, as a function or macro.  */
#define HAVE_ALLOCA 1

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
/* #undef HAVE_ALLOCA_H */

/* Define if you don't have vprintf but do have _doprnt.  */
/* #undef HAVE_DOPRNT */

/* Define if your system has a working fnmatch function.  */
/* #undef HAVE_FNMATCH */

/* Define if the `long double' type works.  */
#define HAVE_LONG_DOUBLE 1

/* Define if you have a working `mmap' system call.  */
/* #undef HAVE_MMAP */

/* Define if your struct stat has st_blksize.  */
/* #undef HAVE_ST_BLKSIZE */

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define as __inline if that's what the C compiler calls it.  */
/* #undef inline */

/* Define if on MINIX.  */
/* #undef _MINIX */

/* Define if the system does not provide POSIX.1 features except
   with this defined.  */
/* #undef _POSIX_1_SOURCE */

/* Define if you need to in order for stat and other things to work.  */
#define _POSIX_SOURCE 1

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

/* Define if the setvbuf function takes the buffering type as its second
   argument and the buffer pointer as the third, as on System V
   before release 3.  */
/* #undef SETVBUF_REVERSED */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
 STACK_DIRECTION > 0 => grows toward higher addresses
 STACK_DIRECTION < 0 => grows toward lower addresses
 STACK_DIRECTION = 0 => direction of growth unknown
 */
/* #undef STACK_DIRECTION */

/* Define if the `S_IS*' macros in <sys/stat.h> do not work properly.  */
#define STAT_MACROS_BROKEN 1

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Define to `int' if <sys/types.h> doesn't define.  */
#define uid_t int

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
/* #undef WORDS_BIGENDIAN */

/* This is always defined.  It enables GNU extensions on systems that
   have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif

/* Define if you have the __argz_count function.  */
/* #undef HAVE___ARGZ_COUNT */

/* Define if you have the __argz_next function.  */
/* #undef HAVE___ARGZ_NEXT */

/* Define if you have the __argz_stringify function.  */
/* #undef HAVE___ARGZ_STRINGIFY */

/* Define if you have the alarm function.  */
/* #undef HAVE_ALARM */

/* Define if you have the bcopy function.  */
/* #undef HAVE_BCOPY */

/* Define if you have the btowc function.  */
/* #undef HAVE_BTOWC */

/* Define if you have the bzero function.  */
/* #undef HAVE_BZERO */

/* Define if you have the dcgettext function.  */
/* #undef HAVE_DCGETTEXT */

/* Define if you have the doprnt function.  */
/* #undef HAVE_DOPRNT */

/* Define if you have the dup2 function.  */
#define HAVE_DUP2 1

/* Define if you have the fseeko function.  */
/* #undef HAVE_FSEEKO */

/* Define if you have the getcwd function.  */
#define HAVE_GETCWD 1

/* Define if you have the getdelim function.  */
/* #undef HAVE_GETDELIM */

/* Define if you have the getpagesize function.  */
#define HAVE_GETPAGESIZE 1

/* Define if you have the isascii function.  */
#define HAVE_ISASCII 1

/* Define if you have the lchown function.  */
/* #undef HAVE_LCHOWN */

/* Define if you have the localtime_r function.  */
/* #undef HAVE_LOCALTIME_R */

/* Define if you have the memchr function.  */
#define HAVE_MEMCHR 1

/* Define if you have the memcpy function.  */
#define HAVE_MEMCPY 1

/* Define if you have the memmove function.  */
#define HAVE_MEMMOVE 1

/* Define if you have the mempcpy function.  */
/* #undef HAVE_MEMPCPY */

/* Define if you have the memset function.  */
#define HAVE_MEMSET 1

/* Define if you have the munmap function.  */
/* #undef HAVE_MUNMAP */

/* Define if you have the nl_langinfo function.  */
/* #undef HAVE_NL_LANGINFO */

/* Define if you have the pow function.  */
/* #undef HAVE_POW */

/* Define if you have the putenv function.  */
#define HAVE_PUTENV 1

/* Define if you have the setenv function.  */
/* #undef HAVE_SETENV */

/* Define if you have the setlocale function.  */
#define HAVE_SETLOCALE 1

/* Define if you have the stpcpy function.  */
/* #undef HAVE_STPCPY */

/* Define if you have the strcasecmp function.  */
#define HAVE_STRCASECMP 1

/* Define if you have the strchr function.  */
#define HAVE_STRCHR 1

/* Define if you have the strdup function.  */
#define HAVE_STRDUP 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the strerror_r function.  */
/* #undef HAVE_STRERROR_R */

/* Define if you have the strftime function.  */
#define HAVE_STRFTIME 1

/* Define if you have the strncasecmp function.  */
#define HAVE_STRNCASECMP 1

/* Define if you have the strpbrk function.  */
#define HAVE_STRPBRK 1

/* Define if you have the strrchr function.  */
#define HAVE_STRRCHR 1

/* Define if you have the strtol function.  */
#define HAVE_STRTOL 1

/* Define if you have the strtoul function.  */
#define HAVE_STRTOUL 1

/* Define if you have the strtoull function.  */
/* #undef HAVE_STRTOULL */

/* Define if you have the strtoumax function.  */
/* #undef HAVE_STRTOUMAX */

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define if you have the <alloca.h> header file.  */
/* #undef HAVE_ALLOCA_H */

/* Define if you have the <argz.h> header file.  */
/* #undef HAVE_ARGZ_H */

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <float.h> header file.  */
#define HAVE_FLOAT_H 1

/* Define if you have the <langinfo.h> header file.  */
/* #undef HAVE_LANGINFO_H */

/* Define if you have the <libintl.h> header file.  */
/* #undef HAVE_LIBINTL_H */

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <locale.h> header file.  */
#define HAVE_LOCALE_H 1

/* Define if you have the <malloc.h> header file.  */
#define HAVE_MALLOC_H 1

/* Define if you have the <memory.h> header file.  */
#define HAVE_MEMORY_H 1

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <nl_types.h> header file.  */
/* #undef HAVE_NL_TYPES_H */

/* Define if you have the <stdlib.h> header file.  */
#define HAVE_STDLIB_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <strings.h> header file.  */
/* #undef HAVE_STRINGS_H */

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/param.h> header file.  */
/* #undef HAVE_SYS_PARAM_H */

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the <utime.h> header file.  */
/* #undef HAVE_UTIME_H */

/* Define if you have the <values.h> header file.  */
#define HAVE_VALUES_H 1

/* Define if you have the <wchar.h> header file.  */
#define HAVE_WCHAR_H 1

/* Define if you have the <wctype.h> header file.  */
/* #undef HAVE_WCTYPE_H */

/* Define if you have the i library (-li).  */
/* #undef HAVE_LIBI */

/* Define if you have the intl library (-lintl).  */
/* #undef HAVE_LIBINTL */

/* Name of package */
#define PACKAGE "textutils"

/* Version number of package */
#define VERSION "2.0"

/* Number of bits in a file offset, on hosts where this is settable.
       case  in
	# HP-UX 10.20 and later
	hpux10.2-90-9* | hpux11-9* | hpux2-90-9*)
	  ac_cv_sys_file_offset_bits=64 ;;
	esac */
/* #undef _FILE_OFFSET_BITS */

/* Define to make fseeko etc. visible, on some hosts. */
/* #undef _LARGEFILE_SOURCE */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define if compiler has function prototypes */
#define PROTOTYPES 1

/* Define if <inttypes.h> exists, doesn't clash with <sys/types.h>,
   and declares uintmax_t.  */
/* #undef HAVE_INTTYPES_H */

/* Define if you have the unsigned long long type. */
#define HAVE_UNSIGNED_LONG_LONG 1

/*   Define to `unsigned long' or `unsigned long long'
   if <inttypes.h> doesn't define. */
#define uintmax_t unsigned long long

/*   Define to `int' if certain system header files doesn't define it. */
/* #undef mode_t */

/*   Define to `long' if certain system header files doesn't define it. */
/* #undef off_t */

/*   Define to `int' if certain system header files doesn't define it. */
/* #undef pid_t */

/*   Define to `unsigned' if certain system header files doesn't define it. */
/* #undef size_t */

/*   Define to `unsigned long' if certain system header files doesn't define it. */
/* #undef ino_t */

/*   Define to `int' if certain system header files doesn't define it. */
#define ssize_t int

/* Define to 1 if assertions should be disabled. */
/* #undef NDEBUG */

/* Define if struct utimbuf is declared -- usually in <utime.h>.
   Some systems have utime.h but don't declare the struct anywhere.  */
/* #undef HAVE_STRUCT_UTIMBUF */

/* Define if there is a member named d_type in the struct describing
   directory headers. */
/* #undef D_TYPE_IN_DIRENT */

/* Define if there is a member named d_ino in the struct describing
   directory headers. */
#undef D_INO_IN_DIRENT

/* Define if this function is declared. */
#define HAVE_DECL_FREE 1

/* Define if this function is declared. */
#define HAVE_DECL_LSEEK 1

/* Define if this function is declared. */
#define HAVE_DECL_MALLOC 1

/* Define if this function is declared. */
#define HAVE_DECL_MEMCHR 1

/* Define if this function is declared. */
#define HAVE_DECL_REALLOC 1

/* Define if this function is declared. */
/* #undef HAVE_DECL_STPCPY */

/* Define if this function is declared. */
#define HAVE_DECL_STRSTR 1

/* Define to rpl_chown if the replacement function should be used. */
#define chown rpl_chown

/* Define to rpl_mktime if the replacement function should be used. */
#define mktime rpl_mktime

/* Define if lstat has the bug that it succeeds when given the zero-length
   file name argument.  The lstat from SunOS4.1.4 and the Hurd as of 1998-11-01)
   do this.  */
/* #undef HAVE_LSTAT_EMPTY_STRING_BUG */

/* Define if stat has the bug that it succeeds when given the zero-length
   file name argument.  The stat from SunOS4.1.4 and the Hurd as of 1998-11-01)
   do this.  */
/* #undef HAVE_STAT_EMPTY_STRING_BUG */

/* Define if the realloc check has been performed.  */
#define HAVE_DONE_WORKING_REALLOC_CHECK 1

/* Define to rpl_realloc if the replacement function should be used. */
/* #undef realloc */

/* Define if the malloc check has been performed.  */
#define HAVE_DONE_WORKING_MALLOC_CHECK 1

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */

/* Define if readdir is found to work properly in some unusual cases.  */
/* #undef HAVE_WORKING_READDIR */

/* Define to rpl_memcmp if the replacement function should be used. */
/* #undef memcmp */

/* Define to rpl_fnmatch if the replacement function should be used. */
#define fnmatch rpl_fnmatch

/* Define if you have the Andrew File System. */
/* #undef AFS */

/* The concatenation of the strings `GNU ', and PACKAGE. */
#define GNU_PACKAGE "GNU textutils"

/* Define to the function xargmatch calls on failures. */
#define ARGMATCH_DIE usage (1)

/* Define to the declaration of the xargmatch failure function. */
#define ARGMATCH_DIE_DECL extern void usage ()

/* Define to 1 if you have the stpcpy function. */
/* #undef HAVE_STPCPY */

/* Define if your locale.h file contains LC_MESSAGES. */
/* #undef HAVE_LC_MESSAGES */

/* Define to 1 if NLS is requested. */
#define ENABLE_NLS 1

/* Define to 1 if you have gettext and don't want to use GNU gettext. */
/* #undef HAVE_GETTEXT */

/* Define as 1 if you have catgets and don't want to use GNU gettext. */
/* #undef HAVE_CATGETS */

#ifdef _MSC_VER 
#include<stddef.h>
#define inline __inline
#define alloca _alloca
//typedef long long __int64
#endif