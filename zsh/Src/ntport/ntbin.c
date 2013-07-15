#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <errno.h>
#include <ntport.h>
#include "zsh.h"

char start_usage[] = 
{ ":\n \
 Similar to cmd.exe's start  \n \
 start [-Ttitle] [-Dpath] [-min] [-max] [-separate] [-shared] \n \
 [-low|normal|realtime|high] program args \n \
 Batch/Cmd files must be started with CMD /K \n"};

extern void make_err_str(unsigned int error,char *buf,int size) ;
/*

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL,
				  error, 
				  MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
				  buf,
				  size,
				  NULL);
	return;

}
*/

int bin_start (char *name, char **v, char *ops, int func) {
	char *cmdstr, *cmdend, *ptr;
	DWORD cmdsize;
	char argv0[MAX_PATH];
	char *savepath;
	char *currdir=NULL;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD dwCreationFlags=CREATE_NEW_CONSOLE;
	int k,cmdlen,j,jj,ret;

	cmdsize = 512;
	cmdstr = heap_alloc(cmdsize);
	cmdend = cmdstr;
	cmdlen = 0;

	memset(&si,0,sizeof(si));
	si.cb = sizeof(si);


	for (k = 0; v[k] != NULL ; k++){

		if ( v[k][0] == '-')  {
			/* various options */
			if( (v[k][1] == 'T') || (v[k][1] == 't'))
				si.lpTitle =&( v[k][2]);
			else if ( (v[k][1] == 'D') || (v[k][1] == 'd'))
				currdir =&( v[k][2]);
			else if (!stricmp(&v[k][1],"MIN") )
				si.wShowWindow = SW_SHOWMINIMIZED;
			else if (!stricmp(&v[k][1],"MAX") )
				si.wShowWindow = SW_SHOWMAXIMIZED;
			else if (!stricmp(&v[k][1],"SEPARATE") )
				dwCreationFlags |= CREATE_SEPARATE_WOW_VDM;
			else if (!stricmp(&v[k][1],"SHARED") )
				dwCreationFlags |= CREATE_SHARED_WOW_VDM;
			else if (!stricmp(&v[k][1],"LOW") )
				dwCreationFlags |= IDLE_PRIORITY_CLASS;
			else if (!stricmp(&v[k][1],"NORMAL") )
				dwCreationFlags |= NORMAL_PRIORITY_CLASS;
			else if (!stricmp(&v[k][1],"HIGH") )
				dwCreationFlags |= HIGH_PRIORITY_CLASS;
			else if (!stricmp(&v[k][1],"REALTIME") )
				dwCreationFlags |= REALTIME_PRIORITY_CLASS;
			else{
				fprintf(stderr,"%s\nSee CMD.EXE for more info\n",start_usage);
				return 1;
			}
		}
		else{ // non-option arg
			break;
		}
	}
	/* 
	 * Stop the insanity of requiring start "tcsh -l"
	 * Option processing now stops at first non-option arg
	 * -amol 5/30/96
	 */
	for (jj=k;v[jj] != NULL; jj++) {
			j=(strlen(v[jj]) + 2);
			if(j + cmdlen > cmdsize) {
				ptr = cmdstr;
				cmdstr = heap_realloc(cmdstr,cmdsize <<1);
				cmdend = cmdstr + (cmdend - ptr);
				cmdsize <<= 1;
			}
			ptr = v[jj];
			while (*ptr) {
				*cmdend++ = *ptr++;
				cmdlen++;
			}
			*cmdend++ = ' ';
			cmdlen++;
	}
	if (k == jj) {
		fprintf(stderr,"%s\nSee CMD.EXE for more info\n",start_usage);
		return 1;
	}
	*cmdend = 0;
	lstrcpyn(argv0,v[k],MAX_PATH-1);
	{
		DWORD len;
		len = GetEnvironmentVariable("PATH",NULL,0);
		savepath = heap_alloc(len+1);
		GetEnvironmentVariable("PATH",savepath,len);

		fix_path_for_child();
	}
	if (! CreateProcess(NULL,
						cmdstr,
						NULL,
						NULL,
						FALSE,
						dwCreationFlags,
						NULL,
						currdir,
						&si,
						&pi) ) {
		ret = GetLastError();

		SetEnvironmentVariable("PATH",savepath);
		heap_free(savepath);

		if (ret == ERROR_BAD_EXE_FORMAT){
			try_shell_ex(&v[k],0);
			errno = ENOEXEC;
		}
		else{
			if (
				( (v[k][0] == '\\') ||(v[k][0] == '/') ) &&
				( (v[k][1] == '\\') ||(v[k][1] == '/') ) &&
				(!v[k+1])
			   )
				try_shell_ex(&v[k],0);
			errno = ENOENT;
		}

		heap_free(cmdstr);

		if (errno) {
			return 1;
		}
	}
	else {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		SetEnvironmentVariable("PATH",savepath);
		heap_free(savepath);
	}


	return 0;
}
int bin_title (char *name, char **v, char *ops, int func) {

	int k;
	char titlebuf[512];
	char errbuf[128],err2[128];

	/*
	if (k=GetConsoleTitle(titlebuf,512) ) {
		titlebuf[k]=0;
//		set(STRoldtitle,SAVE(titlebuf),VAR_READWRITE);
	}
	*/

	memset(titlebuf,0,512);
	for (k = 0; v[k] != NULL ; k++){
		__try {
			 lstrcat(titlebuf,v[k]);
			 lstrcat(titlebuf," ");
		}
		__except(1) {
			fprintf(stderr,"String too long\n");	
			return 1;
		}
	}

	if (!SetConsoleTitle(titlebuf) ) {
			make_err_str(GetLastError(),errbuf,128);
			wsprintf(err2,"Can't set title %s:%s",v[k],errbuf);
			fprintf(stderr,"%s\n",err2);
			return 1;
	}
	return 0;
}
int bin_cls (char *name, char **v, char *ops, int func) {
	extern void NT_ClearScreen_WholeBuffer(void) ;
	NT_ClearScreen_WholeBuffer() ;
	return 0;
}
typedef BOOL (__stdcall *shell_ex_func)(LPSHELLEXECUTEINFO);

static HANDLE hShellDll;
static shell_ex_func pShellExecuteEx;

void init_shell_dll(void) {

	hShellDll = LoadLibrary("Shell32.dll");
	if (hShellDll) {
		pShellExecuteEx = (shell_ex_func)GetProcAddress(
									hShellDll,
									"ShellExecuteEx");
	}
}
void try_shell_ex(char **argv,int exitsuccess) {

	char *prog;
	char cmdstr[1024];
	char err2[256];
	char *ptr;
	short quotespace=0;
	SHELLEXECUTEINFO shinfo;
	unsigned long  mask = SEE_MASK_FLAG_NO_UI;
	BOOL rc, nocmd=0;

	if (isset(WINNTNOASSOCIATIONS)){
		return;
	}
	prog=*argv++;

	ptr = prog;
	while(*ptr) {
		if (*ptr == '/')
			*ptr = '\\';
		ptr++;
	}
	cmdstr[0] = 0;
	if ( (!*argv) &&  (prog[0] == '\\') && (prog[1] == '\\')) {
		mask |= SEE_MASK_CONNECTNETDRV;
		nocmd = 1;
		goto noargs;
	}
	while(*argv) {

		lstrcat(cmdstr," ");
		ptr = *argv;

		while(*ptr) {
			if (*ptr == ' ')
			quotespace=1;
			ptr++;
		}
		if (quotespace) {
			lstrcat(cmdstr,"\"");
			lstrcat(cmdstr,*argv);
			lstrcat(cmdstr,"\"");
		}
		else
			lstrcat(cmdstr,*argv);
		*argv++;
	}
	dprintf("executing %s %s\n",prog,cmdstr);
noargs:

	memset(&shinfo,0,sizeof(shinfo));
	shinfo.cbSize = sizeof(shinfo);
	shinfo.fMask = SEE_MASK_FLAG_DDEWAIT | mask;
	shinfo.hwnd = NULL;
	shinfo.lpVerb = NULL;
	shinfo.lpFile = prog;
	shinfo.lpParameters = nocmd?NULL:&cmdstr[0];
	shinfo.lpDirectory = 0;
	shinfo.nShow = SW_SHOWDEFAULT;


	if ( (rc =pShellExecuteEx(&shinfo)) && exitsuccess)
		ExitProcess(0);
	else if (!rc && !exitsuccess) {
		make_err_str(GetLastError(),cmdstr,512);
		wsprintf(err2,"%s",prog);
		fprintf(stderr,"Failed to start %s: %s\n",prog,cmdstr);
	}
	if (!rc && exitsuccess)
		errno = ENOEXEC;
	else
		errno = 0;

}
