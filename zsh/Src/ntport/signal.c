#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <errno.h>
#include <stdlib.h>
#include <mmsystem.h>
#include "ntport.h"
#include "signal.h"



#define SIGBAD(signo) ( (signo) <=0 || (signo) >=NSIG) 
#define fast_sigmember(a,b) ( (*(a) & (1 << (b-1)) ) )

int generic_handler(DWORD);
int ctrl_handler(DWORD);

typedef struct _child_list {
	DWORD dwProcessId;
	DWORD exitcode;
	struct _child_list *next;
}ChildListNode;

Sigfunc *handlers[NSIG]={0};
static unsigned long gPending[NSIG]={0};
static unsigned long gBlockMask = 0;
static ChildListNode *clist_h=NULL; //head of list
static ChildListNode *clist_t=NULL; // tail of list

static CRITICAL_SECTION sigcritter;
static HANDLE hmainthr;
static HANDLE hsigsusp;
static int __is_suspended;

extern HANDLE __h_con_alarm, __h_con_int;

// must be done before fork;
void nt_init_signals(void) {
	
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrl_handler,TRUE);
	InitializeCriticalSection(&sigcritter);
	clist_t = clist_h;


	if (!DuplicateHandle(GetCurrentProcess(),
					GetCurrentThread(),
					GetCurrentProcess(),
					&hmainthr,
					0,
					FALSE,
					DUPLICATE_SAME_ACCESS)){
		int err = GetLastError();
		ExitProcess(0);
	}
	hsigsusp = CreateEvent(NULL,TRUE,FALSE,NULL);
	__h_con_alarm=CreateEvent(NULL,FALSE,FALSE,NULL);
	__h_con_int=CreateEvent(NULL,FALSE,FALSE,NULL);
	if (!hsigsusp)
		abort();

}
void suspend_main_thread(void) {
//	EnterCriticalSection(&sigcritter);
	SuspendThread(hmainthr);
//	LeaveCriticalSection(&sigcritter);
}
void resume_main_thread(void) {
//	EnterCriticalSection(&sigcritter);
	ResumeThread(hmainthr);
//	LeaveCriticalSection(&sigcritter);
}
int sigaddset(sigset_t *set, int signo) {

	if (SIGBAD(signo)) {
		errno = EINVAL;
		return -1;
	}
	*set |= 1 << (signo-1);
	return 0;
}
int sigdelset(sigset_t *set, int signo) {
	if (SIGBAD(signo)) {
		errno = EINVAL;
		return -1;
	}
	*set &= ~( 1 << (signo-1));

	return 0;
	
}
int sigismember(const sigset_t *set, int signo) {
	if (SIGBAD(signo)) {
		errno = EINVAL;
		return -1;
	}

	return ( (*set & (1 <<(signo-1)) ) != 0);
	
}
void deliver_pending(void) {
	unsigned long temp;
	int sig=1;

	temp = ~gBlockMask;
	while(temp && (sig < NSIG)) {

		if (temp & 0x01){
			if (gPending[sig]){
				gPending[sig]=0;
				generic_handler(sig);
			}
		}
		temp >>= 1;
		sig++;
	}
}
int sigprocmask(int how, const sigset_t *set, sigset_t*oset) {

//	EnterCriticalSection(&sigcritter);
	if (oset)
		*oset = gBlockMask;
	if (set) {
		switch (how) {
			case SIG_BLOCK:
				gBlockMask |= *set;
				break;
			case SIG_UNBLOCK:
				gBlockMask &= (~(*set));
				break;
			case SIG_SETMASK:
				gBlockMask = *set;
				break;
			default:
				break;
		}
	}
	if (how != SIG_BLOCK)
		deliver_pending();
//	LeaveCriticalSection(&sigcritter);

	return 0;

}
int sigsuspend(const sigset_t *mask) {
	sigset_t omask;


	EnterCriticalSection(&sigcritter);
	__is_suspended =1;
	LeaveCriticalSection(&sigcritter);

	sigprocmask(SIG_SETMASK,mask,&omask);

	WaitForSingleObject(hsigsusp,INFINITE);
	ResetEvent(hsigsusp);

	EnterCriticalSection(&sigcritter);
	__is_suspended =0;
	LeaveCriticalSection(&sigcritter);

	sigprocmask(SIG_SETMASK,&omask,0);
	errno = EINTR;
	return -1;

}
int sigaction(int signo, const struct sigaction *act, struct sigaction *oact) {

	if (SIGBAD(signo)) {
		errno = EINVAL;
		return -1;
	}
//	EnterCriticalSection(&sigcritter);

	if(oact){
			oact->sa_handler = handlers[signo];
			oact->sa_mask = 0;
			oact->sa_flags =0;
	}
	if (act)
		handlers[signo]=act->sa_handler;

	return 0;
//	LeaveCriticalSection(&sigcritter);
	
}
void inc_pending(int signo) {
//	EnterCriticalSection(&sigcritter);
	gPending[signo] += 1;
//	LeaveCriticalSection(&sigcritter);
}
int ctrl_handler(DWORD event) {
	dprintf("event %d\n",event);

	return generic_handler(event+1);
}
int generic_handler(DWORD signo) {

	int blocked=0;

	if (SIGBAD(signo) )
		return FALSE;
//	EnterCriticalSection(&sigcritter);
	switch (signo) {
		case SIGINT:
			if (handlers[signo] != SIG_IGN){
				if (fast_sigmember(&gBlockMask,signo) ) {
					inc_pending(signo);
					blocked=1;
				}
				else if (handlers[signo] == SIG_DFL)
					ExitProcess(0xC000013AL);
				else{
					handlers[signo](signo);
					SetEvent(__h_con_int);
				}
			}
			break;
		case SIGBREAK:
			if (handlers[signo] != SIG_IGN){
				if (fast_sigmember(&gBlockMask,signo) ) {
					inc_pending(signo);
					blocked=1;
				}
				else if (handlers[signo] == SIG_DFL)
					ExitProcess(0xC000013AL);
				else
					handlers[signo](signo);
			}
			break;
		case SIGHUP: //CTRL_CLOSE_EVENT
			if (handlers[signo] != SIG_IGN){
				if (fast_sigmember(&gBlockMask,signo) ) {
					inc_pending(signo);
					blocked=1;
				}
				else if (handlers[signo] == SIG_DFL)
					ExitProcess(604);
				else
					handlers[signo](signo);
			}
			break;
		case SIGTERM: //CTRL_LOGOFF_EVENT
			if (handlers[signo] != SIG_IGN){
				if (fast_sigmember(&gBlockMask,signo) ) {
					inc_pending(signo);
					blocked=1;
				}
				else if (handlers[signo] == SIG_DFL)
					ExitProcess(604);
				else
					handlers[signo](signo);
			}
			else
				ExitProcess(604);
			break;
		case SIGKILL: //CTRL_SHUTDOWN_EVENT
			if (handlers[signo] != SIG_IGN){
				if (fast_sigmember(&gBlockMask,signo) ) {
					inc_pending(signo);
					blocked=1;
				}
				else if (handlers[signo] == SIG_DFL)
					ExitProcess(604);
				else
					handlers[signo](signo);
			}
			else
				ExitProcess(604);
			break;
		case SIGALRM:
			if (handlers[signo] != SIG_IGN){
				if (fast_sigmember(&gBlockMask,signo) ) {
					inc_pending(signo);
					blocked=1;
				}
				else if (handlers[signo] == SIG_DFL)
					ExitProcess(604);
				else
					handlers[signo](signo);
			}
			break;
		case SIGCHLD:
			if (handlers[signo] != SIG_IGN){
				if (fast_sigmember(&gBlockMask,signo) ) {
					inc_pending(signo);
					blocked=1;
				}
				else if (handlers[signo] != SIG_DFL)
					handlers[signo](signo);
			}
			break;
		default:
			ExitProcess(604);
			break;
	}
	if (!blocked && __is_suspended)
		SetEvent(hsigsusp);
//	LeaveCriticalSection(&sigcritter);
	return TRUE;
}
Sigfunc *_nt_signal(int signal, Sigfunc * handler) {

	Sigfunc *old;

	if (SIGBAD(signal)) {
		errno = EINVAL;
		return SIG_ERR;
	}

//	EnterCriticalSection(&sigcritter);

	old = handlers[signal];
	handlers[signal] = handler;

//	LeaveCriticalSection(&sigcritter);

	return old;
}
int waitpid(pid_t pid, int *statloc, int options) {
	
	ChildListNode *temp;
	int retcode;

	errno = EINVAL;
	if (pid != -1)
		return -1;

	EnterCriticalSection(&sigcritter);
		if (!clist_h)
			retcode =0;
		else {
			retcode = clist_h->dwProcessId;
			if (statloc) *statloc = (clist_h->exitcode & 0x00FF);
			temp = clist_h;
			clist_h = clist_h->next;
			ffree(temp);
		}
	LeaveCriticalSection(&sigcritter);

	errno = 0;
	return retcode;
	
}
HANDLE __halarm=0;
unsigned int __alarm_set=0;

void CALLBACK alarm_callback( unsigned long interval) {

	int rc;

	rc = WaitForSingleObject(__halarm,interval*1000);
	if (rc != WAIT_TIMEOUT)
		return ;

	SetEvent(__h_con_alarm);
	__alarm_set = 0;
	return;
	
	// consoleread() now waits for above event, and calls generic_handler to
	// handle SIGALRM in the main thread. That helps me avoid
	// problems with  fork() when we are in a secondary thread.
	//
	// This means sched, periodic etc will not be signalled unless consoleread
	// is called, but that's a reasonable risk, i think.
	// -amol 4/10/97

#if 0
	EnterCriticalSection(&sigcritter);
	suspend_main_thread();
	__try {
		generic_handler(SIGALRM);
	}
	__except(1) {
		;
	}
	resume_main_thread();
	LeaveCriticalSection(&sigcritter);

#endif 0
}
unsigned int alarm(unsigned int seconds) {

	unsigned int temp;
	static unsigned int prev_val=0;
	HANDLE ht;
	DWORD tid;


	if (!__halarm) {
		__halarm=CreateEvent(NULL,FALSE,FALSE,NULL);
	}
	if(__alarm_set )
		SetEvent(__halarm);

	if (!seconds){
		__alarm_set=0;
		return 0;
	}
	__alarm_set = 1;

	ht = CreateThread(NULL,0, (LPTHREAD_START_ROUTINE)alarm_callback,
					(void*)seconds,0,&tid);
	if (ht)
		CloseHandle(ht);
	
	temp = prev_val;
	prev_val = seconds*1000;

	return temp;
}
void add_to_child_list(DWORD dwpid,DWORD exitcode) {
//	EnterCriticalSection(&sigcritter);
	if (clist_h == NULL) {
		clist_h = malloc(sizeof(ChildListNode));
		if (!clist_h)
			goto end;
		clist_h->dwProcessId = dwpid;
		clist_h->exitcode = exitcode;
		clist_h->next= NULL;
		clist_t = clist_h;
	}
	else {
		clist_t->next = malloc(sizeof(ChildListNode));
		if (!clist_t->next)
			goto end;
		clist_t = clist_t->next;
		clist_t->dwProcessId= dwpid;
		clist_h->exitcode = exitcode;
		clist_t->next = NULL;	
	}
end:
	;
//	LeaveCriticalSection(&sigcritter);
}
void sig_child_callback(DWORD pid,DWORD exitcode) {
	
	EnterCriticalSection(&sigcritter);
	add_to_child_list(pid,exitcode);
	suspend_main_thread();
	//
	// pchild() tries to reset(), which crashes the thread
	//
	__try {
		generic_handler(SIGCHLD);
	}
	__except(1) {
		;
	}
	resume_main_thread();
	LeaveCriticalSection(&sigcritter);

}
struct thread_args {
	DWORD pid;
	HANDLE hproc;
};
void sigchild_thread(struct thread_args *args) {

	DWORD exitcode=0;
	WaitForSingleObject(args->hproc,INFINITE);
	GetExitCodeProcess(args->hproc,&exitcode);
	CloseHandle(args->hproc);
	sig_child_callback(args->pid,exitcode);
	ffree(args);
}
void start_sigchild_thread(HANDLE hproc, DWORD pid) {

	struct thread_args *args=fmalloc(sizeof(struct thread_args));
	DWORD tid;
	HANDLE hthr;
	args->hproc = hproc;
	args->pid = pid;

	hthr = CreateThread(NULL,
							0,
							(LPTHREAD_START_ROUTINE)sigchild_thread,
							(LPVOID)args,
							0,
							&tid);


	CloseHandle(hthr);

}
int kill (int pid, int sig) {

	HANDLE hproc;
	int ret =0;

	errno = EPERM;

	switch(sig) {
		case 0:
		case 7:
			hproc = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
			if (hproc  == NULL) {
				errno = ESRCH;
				ret = -1;
			}
			if (sig == 7) {
				if (!TerminateProcess(hproc,0xC000013AL) ) {
					ret = -1;
				}
			}
			CloseHandle(hproc);
			break;
		case 1:
			if (!GenerateConsoleCtrlEvent(CTRL_C_EVENT,pid)) 
				ret = -1;
			break;
		case 2:
			if (!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT,pid)) 
				ret = -1;
			break;
		default:
			break;
	}
	return ret;
}
//
// nice(niceness)
//
// where niceness is an integer in the range -6 to +7
//
// A usual foreground process starts at level 9 in the chart below
//
// the range -6 to +7 takes it from Base priority 15 down to 2. 
//
// Note that level 1 or > 15 are not allowed.
//
// Priority Level 11 (niceness -2) or greater affects system performance, 
//	so use with care.
//
// niceness defaults to  +4, which is lowest for background normal class.
// As in unix, +ve niceness indicates lower priorities.

/***************************************************************************
Niceness    Base    Priority class/thread priority

            1    Idle, normal, or high class,    THREAD_PRIORITY_IDLE

+7          2    Idle class,                     THREAD_PRIORITY_LOWEST
+6          3    Idle class,                     THREAD_PRIORITY_BELOW_NORMAL
+5          4    Idle class,                     THREAD_PRIORITY_NORMAL
+4          5    Background normal class,        THREAD_PRIORITY_LOWEST
                    Idle class,                  THREAD_PRIORITY_ABOVE_NORMAL
+3          6    Background normal class,        THREAD_PRIORITY_BELOW_NORMAL
                    Idle class,                  THREAD_PRIORITY_HIGHEST
+2          7    Foreground normal class,        THREAD_PRIORITY_LOWEST
                    Background normal class,     THREAD_PRIORITY_NORMAL
+1          8    Foreground normal class,        THREAD_PRIORITY_BELOW_NORMAL
                    Background normal class,     THREAD_PRIORITY_ABOVE_NORMAL
 0          9    Foreground normal class,        THREAD_PRIORITY_NORMAL
                    Background normal class,     THREAD_PRIORITY_HIGHEST
-1          10   Foreground normal class,        THREAD_PRIORITY_ABOVE_NORMAL
-2          11    High class,                    THREAD_PRIORITY_LOWEST
                    Foreground normal class,     THREAD_PRIORITY_HIGHEST
-3          12    High class,                    THREAD_PRIORITY_BELOW_NORMAL
-4          13    High class,                    THREAD_PRIORITY_NORMAL
-5          14    High class,                    THREAD_PRIORITY_ABOVE_NORMAL
-6          15    Idle, normal, or high class,   THREAD_PRIORITY_TIME_CRITICAL 
                  High class,                    THREAD_PRIORITY_HIGHEST


    16    Real-time class, THREAD_PRIORITY_IDLE
    22    Real-time class, THREAD_PRIORITY_LOWEST
    23    Real-time class, THREAD_PRIORITY_BELOW_NORMAL
    24    Real-time class, THREAD_PRIORITY_NORMAL
    25    Real-time class, THREAD_PRIORITY_ABOVE_NORMAL
    26    Real-time class, THREAD_PRIORITY_HIGHEST
    31    Real-time class, THREAD_PRIORITY_TIME_CRITICAL
****************************************************************************/
int nice(int niceness) {

    DWORD pclass;
    int priority;

    if (niceness < -6 || niceness > 7) {
        errno = EPERM;
        return -1;
    }
    switch (niceness) {
        case 7:
            pclass = IDLE_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_LOWEST;
            break;
        case 6:
            pclass = IDLE_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case 5:
            pclass = IDLE_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_NORMAL;
            break;
        case 4:
            pclass = IDLE_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case 3:
            pclass = IDLE_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_HIGHEST;
            break;
        case 2:
            pclass = NORMAL_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_LOWEST;
            break;
        case 1:
            pclass = NORMAL_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case (-1):
            pclass = NORMAL_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case (-2):
            pclass = NORMAL_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_HIGHEST;
            break;
        case (-3):
            pclass = HIGH_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case (-4):
            pclass = HIGH_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_NORMAL;
            break;
        case (-5):
            pclass = HIGH_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case (-6):
            pclass = HIGH_PRIORITY_CLASS;
            priority = THREAD_PRIORITY_HIGHEST;
            break;
        default:
            break;
    }

    if (!SetPriorityClass(GetCurrentProcess(),pclass)){
        int err = GetLastError() ;
        errno = EPERM;
        return -1;
    }
    if (!SetThreadPriority(GetCurrentThread(),priority)){
        int err = GetLastError() ;
        errno = EPERM;
        return -1;
    }
    return 0;
}
