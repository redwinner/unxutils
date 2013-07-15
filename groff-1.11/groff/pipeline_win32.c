#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef WIN32
#include <process.h>
#include <fcntl.h>
#include <io.h>
int pipe(int *pdes);
#endif



#ifndef errno
extern int errno;
#endif

extern char *strerror();

#ifdef _POSIX_VERSION

#include <sys/wait.h>

#define PID_T pid_t

#else /* not _POSIX_VERSION */

/* traditional Unix */

#define WIFEXITED(s) (((s) & 0377) == 0)
#define WIFSTOPPED(s) (((s) & 0377) == 0177)
#define WIFSIGNALED(s) (((s) & 0377) != 0 && (((s) & 0377) != 0177))
#define WEXITSTATUS(s) (((s) >> 8) & 0377)
#define WTERMSIG(s) ((s) & 0177)
#define WSTOPSIG(s) (((s) >> 8) & 0377)

#ifndef WCOREFLAG
#define WCOREFLAG 0200
#endif

#define PID_T int

#endif /* not _POSIX_VERSION */


#include "pipeline.h"

#ifdef __STDC__
#define P(parms) parms
#else
#define P(parms) ()
#define const /* as nothing */
#endif

#define error c_error
extern void error P((const char *, const char *, const char *, const char *));
extern void c_fatal P((const char *, const char *, const char *, const char *));

static void sys_fatal P((const char *));
static const char *xstrsignal P((int));
static char *iitoa P((int));

int run_pipeline(ncommands, commands)
int ncommands;
char ***commands;
{
	int i;
	int last_input = 0, hstdout,hstdin;
//	PID_T pids[MAX_COMMANDS];
	int outdes[2];
	int ret = 0, status;
	int proc_count = ncommands;
	PID_T pid;
	
	// save  stdin and stdout
	if ( (hstdout = dup(_fileno(stdout))) < 0)
		sys_fatal("dup");
	if ( ( hstdin = dup(_fileno(stdin))) < 0)
		sys_fatal("dup");
	
	for (i = 0; i < ncommands; i++) {
		
		if (i != ncommands - 1) {
			if (pipe(outdes) < 0)
				sys_fatal("pipe");
			
		}
		
		
		if (last_input != 0) {
			if ( dup2(last_input, _fileno(stdin)) < 0)
				sys_fatal("dup2 in");
			if (close(last_input) < 0)
				sys_fatal("close");
		}
		
		if (i != ncommands - 1) {
			if ( dup2(outdes[1], _fileno(stdout)) < 0)
				sys_fatal("dup2 out");
			if (close(outdes[1]))
				sys_fatal("close");
		} else {
			// restore stdout	    
			if ( dup2(hstdout , _fileno(stdout)) < 0)
				sys_fatal("dup2 stdout");
			close(hstdout);
		}
		
		
		pid=spawnvp(_P_NOWAIT, commands[i][0], commands[i]);
		if(pid < 0) {
			error("couldn't exec %1: %2", commands[i][0],
				strerror(errno), (char *)0);
			fflush(stderr);		/* just in case error() doesn't */
			_exit(EXEC_FAILED_EXIT_STATUS);
			
		}
		
		//        while( (nread=read(outdes[0], buf, BUFSIZE)) >0)
		//			write(indes[1], buf, nread);
		
		
		last_input=outdes[0];
		
	}

// waiting for the end of the pipeline
	_cwait(&status, pid, 0);

// restore  stdin
	if ( dup2(hstdin , _fileno(stdin)) < 0)
		sys_fatal("dup2 stdin");
	close(hstdin);
	
	return ret=status;
}

static void sys_fatal(s)
const char *s;
{
	c_fatal("%1: %2", s, strerror(errno), (char *)0);
}

static char *iitoa(n)
int n;
{
	static char buf[12];
	sprintf(buf, "%d", n);
	return buf;
}

static const char *xstrsignal(n)
int n;
{
	static char buf[sizeof("Signal ") + 1 + sizeof(int)*3];
#ifdef NSIG
#ifdef SYS_SIGLIST_DECLARED
	if (n >= 0 && n < NSIG && sys_siglist[n] != 0)
		return sys_siglist[n];
#endif /* SYS_SIGLIST_DECLARED */
#endif /* NSIG */
	sprintf(buf, "Signal %d", n);
	return buf;
}

#ifdef WIN32
int pipe(int *pdes)
{
	return _pipe(pdes,  65535, O_BINARY | O_NOINHERIT );
	
}
#endif