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
#ifndef SIGNAL_H
#define SIGNAL_H


#define NSIG 23     

// These must be CTRL_xxx_EVENT+1 (in wincon.h)
//
#define SIGINT		1 
#define SIGBREAK 	2
#define SIGHUP		3 //CTRL_CLOSE_EVENT
// 3 and 4 are reserved. hence we can't use 4 and 5
#define	SIGTERM		6 // ctrl_logoff
#define SIGKILL		7 // ctrl_shutdown

#define SIGILL		8 
#define SIGFPE		9	
#define SIGALRM		10
#define SIGWINCH	11
#define SIGSEGV 	12	
#define SIGSTOP 	13
#define SIGPIPE 	14
#define SIGCHLD 	15
#define SIGCONT		16 
#define SIGTSTP 	18
#define SIGTTOU 	19
#define SIGTTIN 	20
#define SIGABRT 	22	

#define SIGQUIT SIGBREAK

/* signal action codes */

#define SIG_DFL (void (*)(int))0	   /* default signal action */
#define SIG_IGN (void (*)(int))1	   /* ignore signal */
#define SIG_SGE (void (*)(int))3	   /* signal gets error */
#define SIG_ACK (void (*)(int))4	   /* acknowledge */


/* signal error value (returned by signal call on error) */

#define SIG_ERR (void (*)(int))-1	   /* signal error value */


#define SIG_BLOCK 0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2

#undef signal
#define signal _nt_signal

typedef unsigned long sigset_t;
typedef void Sigfunc (int);

struct sigaction {
	Sigfunc *sa_handler;
	sigset_t sa_mask;
	int sa_flags;
};


#define sigemptyset(ptr) (*(ptr) = 0)
#define sigfillset(ptr)  ( *(ptr) = ~(sigset_t)0,0)

/* Function prototypes */

void (* _nt_signal(int, void (*)(int)))(int);

int sigaddset(sigset_t*, int);
int sigdelset(sigset_t*,int);
unsigned int alarm(unsigned int);

int sigismember(const sigset_t *set, int);
int sigprocmask(int ,const sigset_t*,sigset_t*);
int sigaction(int, const struct sigaction *, struct sigaction*);
int sigsuspend(const sigset_t *sigmask);

#define WNOHANG 0
#define WUNTRACED 1

#define WIFEXITED(a) 1
#define WEXITSTATUS(a) (a)
//#define WIFSIGNALED(a) (( ( (((unsigned long)(a)) >>24) & 0xC0) !=0) )
//
// this needs to be fixed ??
#define WIFSIGNALED(a) ( ((((unsigned long)(a)) & 0xC0000000 ) != 0))
#define WTERMSIG(a) ( ((unsigned long)(a))==0xC000013AL?SIGINT:SIGSEGV)
#define WCOREDUMP(a) 0
#define WIFSTOPPED(a) 0
#define WSTOPSIG(a) 0

int waitpid(pid_t, int*,int);
int times(struct tms*);
  
#endif SIGNAL_H
