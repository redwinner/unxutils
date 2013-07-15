//
// Copyright(c) 1997 Amol Deshpande
// amold@microsoft.com
// Redistribution in source or binary form is permitted as long as you 
// retain this notice in the distribution
//
// The memory allocator herein is part of the tcsh shell distribution. 
// The original tcsh code is under its own copyright. Get the source from
// ftp.deshaw.com/pub to figure it out.
//
// 
// The fork() implementation borrows heavily from the cygnus gnu-win32
// project's implementation. Check out www.cygnus.com for more information.
// 
//
// There is one restriction that I impose on any users of this code. If you
// use it in your own application, your application *must* be freely 
// redistributable in source form also.
// (I suppose borrowing ideas from the cygnus code makes this a requirement,
// since that code is GPL'd. However, I want to explicitly make it clear that 
// this is FREE software).
//
// You are specifically prohibited from enhancing or fixing bugs in this 
// implementation and selling the resultant product. If you make any changes
// that fix bugs in or enhance this code, you *must* send me a copy.
//
// I retain all rights to this software, except for the tcsh code.
//
// Amol Deshpande and the Zsh Development Group specifically disclaim any
// warranties, including, but not limited to, the implied warranties of
// merchantability and fitness for a particular purpose.  The software
// provided hereunder is on an "as is" basis, and Amol Deshpande and the
// Zsh Development Group have no obligation to provide maintenance,
// support, updates, enhancements, or modifications.
//
//
// And finally,
// Microsoft Corporation has nothing to do with this code. 
//
//
// ntport.h
// the main header.
//
//
#ifndef NTPORT_H
#define NTPORT_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <direct.h>
#include <memory.h>
#include "dirent.h"

#pragma data_seg(".fusrdata")
#define INIT_ZERO =0
#define INIT_ZERO_STRUCT ={0}
#define malloc fmalloc
#define free   ffree
#define realloc frealloc
#define calloc fcalloc

#define heap_alloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s))
#define heap_free(p) HeapFree(GetProcessHeap(),0,(p))
#define heap_realloc(p,s) HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(p),(s))

#pragma warning(disable:4018)
#define HAVENOUTMP
#define HAVENOLIMIT
#define FUNCPROTO

/* os-dependent stuff. belongs in tc.os.h, but I never said this would be
pretty */

#ifdef S_IFMT
# if !defined(S_ISDIR) && defined(S_IFDIR)
#  define S_ISDIR(a)	(((a) & S_IFMT) == S_IFDIR)
# endif	/* ! S_ISDIR && S_IFDIR */
# if !defined(S_ISCHR) && defined(S_IFCHR)
#  define S_ISCHR(a)	(((a) & S_IFMT) == S_IFCHR)
# endif /* ! S_ISCHR && S_IFCHR */
# if !defined(S_ISBLK) && defined(S_IFBLK)
#  define S_ISBLK(a)	(((a) & S_IFMT) == S_IFBLK)
# endif	/* ! S_ISBLK && S_IFBLK */
# if !defined(S_ISREG) && defined(S_IFREG)
#  define S_ISREG(a)	(((a) & S_IFMT) == S_IFREG)
# endif	/* ! S_ISREG && S_IFREG */
# if !defined(S_ISFIFO) && defined(S_IFIFO)
#  define S_ISFIFO(a)	(((a) & S_IFMT) == S_IFIFO)
# endif	/* ! S_ISFIFO && S_IFIFO */
# if !defined(S_ISNAM) && defined(S_IFNAM)
#  define S_ISNAM(a)	(((a) & S_IFMT) == S_IFNAM)
# endif	/* ! S_ISNAM && S_IFNAM */
# if !defined(S_ISLNK) && defined(S_IFLNK)
#  define S_ISLNK(a)	(((a) & S_IFMT) == S_IFLNK)
# endif	/* ! S_ISLNK && S_IFLNK */
# if !defined(S_ISSOCK) && defined(S_IFSOCK)
#  define S_ISSOCK(a)	(((a) & S_IFMT) == S_IFSOCK)
# endif	/* ! S_ISSOCK && S_IFSOCK */
#endif /* S_IFMT */

/* port defines */
#define getpid (int)GetCurrentProcessId
#define mygetpgrp() 0  
#define tcgetpgrp GetCurrentProcessId
#define tcsetpgrp(a,b) 0
#define setpgid(a,b) 0

#define strlen(a) (int)lstrlen((a))

#define close(a) nt_close(a)
#define execv(a,b) nt_exec((a),(b))

#define open  nt_open
#define read(f,b,n) nt_read((f),(b),(n))
#define write(f,b,n) nt_write((f),(b),(n))
#define creat(f,m) nt_creat((f),(m))
#define _exit(a) ExitProcess((a))

#define chdir(a) nt_chdir(a)

#define fgetc(a) nt_fgetc(a)
#define fputs(a,b) nt_fputs((a),(b))
#define putc(a,b) nt_putc((char)(a),(b))
#define fflush(a) nt_fflush((a))
#define fputc(a,b) nt_fputc((char)(a),(b))
#define fprintf nt_fprintf
#define puts(a) nt_puts(a)
#define putchar(a) nt_putchar((char)(a))
#define fclose(p) nt_fclose(p)
#define _get_osfhandle __nt_get_osfhandle
#define _open_osfhandle __nt_open_osfhandle
#define clearerr nt_clearerr
#define dup2  nt_dup2
#define fdopen nt_fdopen
#define fgets  nt_fgets
#define fileno nt_fileno
#define fopen  nt_fopen
#define fread  nt_fread
#define fseek  nt_fseek
#define ftell  nt_ftell
#define fwrite nt_fwrite
#define isatty nt_isatty
#define lseek  nt_lseek
#define printf nt_printf
#define access nt_access
#define fstat(a,b)  nt_fstat((a),(b))
#define stat(a,b)  nt_stat((a),(b))
#define setvbuf(a,b,c,d) 
#define setpgrp(a,b) (-1)

#undef stdin
#undef stdout
#undef stderr
#define stdin ((FILE*)my_stdin)
#define stdout ((FILE*)my_stdout)
#define stderr ((FILE*)my_stderr)

#define dup(f) nt_dup((f))
#define sleep(a) Sleep((a)*1000)

#define getwd(a) forward_slash_get_cwd(a)


#define L_SET   SEEK_SET
#define L_XTND  SEEK_END
#define L_INCR  SEEK_CUR
#define S_IXUSR S_IEXEC
#define S_IXGRP S_IEXEC
#define S_IXOTH S_IEXEC

#define NOFILE  64
#define ARG_MAX 1024
#define MAXSIG  NSIG
#define NCARGS  ARG_MAX

/*
mode Value	Checks File For

00	Existence only
02 	Write permission
04	Read permission
06	Read and write permission
*/
#define F_OK 0
#define X_OK 1
#define W_OK 2
#define R_OK 4
#define XD_OK 9 //executable and not directory


#define IS_WINDOWS_9x() (gdwPlatform != VER_PLATFORM_WIN32_NT)
typedef int uid_t;
typedef int gid_t;
typedef int pid_t;
typedef int mode_t;
typedef void sigret_t;
typedef long rlim_t;
typedef unsigned char u_char;
struct timeval{
	long tv_sec;
	long tv_usec;
};
struct timezone{
	int tz_minuteswest;
	int dsttime;
};
struct rusage {

	 struct timeval ru_utime; /* user time used */
	 struct timeval ru_stime; /* system time used */
	 long ru_maxrss;          /* maximum resident set size */
	 long ru_ixrss;      /* integral shared memory size */
	 long ru_idrss;      /* integral unshared data size */
	 long ru_isrss;      /* integral unshared stack size */
	 long ru_minflt;          /* page reclaims */
	 long ru_majflt;          /* page faults */
	 long ru_nswap;      /* swaps */
	 long ru_inblock;         /* block input operations */
	 long ru_oublock;         /* block output operations */
	 long ru_msgsnd;          /* messages sent */
	 long ru_msgrcv;          /* messages received */
	 long ru_nsignals;        /* signals received */
	 long ru_nvcsw;      /* voluntary context switches */
	 long ru_nivcsw;          /* involuntary context switches */
};
struct passwd {
	  char    *pw_name;       /* user name */
	  char    *pw_passwd;     /* user password */
	  uid_t   pw_uid;         /* user id */
	  gid_t   pw_gid;         /* group id */
	  char    *pw_gecos;      /* real name */
	  char    *pw_dir;        /* home directory */
	  char    *pw_shell;      /* shell program */
};  
struct group {
	  char    *gr_name;        /* group name */
	  char    *gr_passwd;      /* group password */
	  gid_t   gr_gid;          /* group id */
	  char    **gr_mem;        /* group members */
};

/* ntport.c */
char *			ttyname(int);
struct passwd*  getpwuid(uid_t ) ;
struct group *  getgrgid(gid_t ) ;
struct passwd*  getpwnam(char* ) ;
struct group*  getgrnam(char* ) ;
gid_t 			getuid(void) ;
gid_t 			getgid(void) ;
gid_t 			geteuid(void) ;
gid_t 			getegid(void) ;
void			nt_free(void *);

#ifdef NTDBG
extern void dprintf(char *,...);
#define DBreak() __asm {int 3}
#else
#define dprintf if(0)printf
#define DBreak()
#endif NTDBG

#define USE(a) (void)(a)
#define pipe(a) nt_pipe(a)


void nt_init(void);
void gethostname(char*,int);
void set_default_path(char**);
int  nt_read(int, unsigned char*,int);
int  force_read(int, unsigned char*,int);
int  tc_putc(char,FILE*);
int  nt_write(int, unsigned char*,int);
int  nt_creat(char*,int);
int  nt_close(int);
int  nt_open(char*,int ,...);
char* forward_slash_get_cwd(char * ,int) ;
void  nt_execve(char *,char**,char**);
void  nt_exec(char *,char**);
int   pipe(int*);

extern void fix_path_for_child(void) ;
extern int copy_quote_and_fix_slashes(char *,char *, int * );
extern void concat_args_and_quote(char **,char **,unsigned int *, char **,
    unsigned int *) ;


extern int nt_chdir(char*);
extern void caseify_pwd(char *);
extern int is_pathext(char *);

/* stdio.c */
int  nt_fgetc(FILE*);
int	 nt_dup(int);
int  nt_fputs(char*,FILE*);
int  nt_putc(char,FILE*);
int  nt_fflush(FILE*);
int  nt_fputc(char, FILE*);
int  nt_fprintf(FILE*,char*,...);
int  nt_puts(char*);
int  nt_putchar(char);
int  nt_fclose(FILE*);
int  nt_fputs(char *, FILE*);
unsigned long  __nt_get_osfhandle(int);
int __nt_open_osfhandle(long, int);
int nt_clearerr(FILE*);
int  nt_dup2(int,int );
FILE* nt_fdopen(int,char*);
char *  nt_fgets(char *,int, FILE*);
int nt_fileno(FILE*);
FILE *nt_fopen(char *,char*);
int nt_fread(void *,size_t,size_t,FILE*);
int nt_fwrite(void*,size_t,size_t,FILE*);
int nt_fseek(FILE*,long,int);
long nt_ftell(FILE*);
int  nt_isatty(int);
int  nt_lseek(int,long,int);
int nt_printf(char*,...);
int nt_access(char*,int);
int nt_fstat(int, struct stat *) ;
int nt_stat(char *, struct stat *) ;
extern void copy_fds(void);
extern void restore_fds(void);
extern void start_sigchild_thread(HANDLE,DWORD);
extern void close_copied_fds(void);

#ifndef STDIO_C
extern void *my_stdin,*my_stdout,*my_stderr;
#endif STDIO_C

int   gettimeofday(struct timeval *, struct timezone *) ;
char * getlogin(void);

/* fork.c */
extern int fork_init(void);
extern int fork(void);
extern void *fmalloc(int);
extern void ffree(void *);
extern void *frealloc(void*,int);
extern void *fcalloc(int,int);

/* ntbin.c */
int bin_start (char *name, char **argv, char *ops, int func) ;
int bin_title (char *name, char **argv, char *ops, int func) ;
int bin_cls (char *name, char **argv, char *ops, int func) ;
/* ps.c */
int bin_ps (char *name, char **argv, char *ops, int func) ;
int bin_shutdown(char *name, char **argv, char *ops, int func) ;

/*globals.c*/
int is_gui(char*);
int is_9x_gui(char*);

#define execve(a,b,c) nt_execve((a),(b),(c))

typedef struct param *Param;
char * semicolonarrgetfn (Param pm);
void semicolonarrsetfn (Param pm, char *x);

extern void init_shell_dll(void);
extern void try_shell_ex(char**,int);

extern void init_plister(void);
// global vars
extern unsigned short __nt_want_vcode;
extern unsigned int oldcp, dontmuckwithmycodepage;
extern int ntvirtualbind[];
extern DWORD gdwPlatform;
extern OSVERSIONINFO gosver;
extern int gisWin95;
// bogus
#define STRUCT_UTMP long
#define getppid() 0

struct tms {
	clock_t tms_utime;
	clock_t tms_stime;
	clock_t tms_cutime;
	clock_t tms_cstime;
};
extern int kill(int,int);
extern int nice(int);

#endif NTPORT_H
