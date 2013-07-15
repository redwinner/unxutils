#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winperf.h>
#include <tlhelp32.h>
#include <zsh.h>
#include <errno.h>

#define p_malloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s))
#define p_realloc(s,p) HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s),(p))
#define p_free(p) HeapFree(GetProcessHeap(),0,(p))

#define REGKEY_PERF         "software\\microsoft\\windows nt\\currentversion\\perflib"
#define REGSUBKEY_COUNTERS  "Counters"
#define PROCESS_COUNTER     "process"
#define PROCESSID_COUNTER   "id process"

typedef struct _pslist {
	DWORD pid;
	HANDLE hwnd;
	char exename[MAX_PATH];
	char title[80];
}pslist;


typedef BOOL (WINAPI *walker)(HANDLE,LPPROCESSENTRY32);
typedef HANDLE (WINAPI *create_snapshot)(DWORD,DWORD);
static walker proc32First;
static walker proc32Next;
static create_snapshot createSnapshot;

typedef BOOL (WINAPI *enumproc)(DWORD *,DWORD,DWORD *);
typedef BOOL (WINAPI *enummod)(HANDLE,HMODULE*,DWORD,DWORD*);
typedef DWORD(WINAPI *getfilename_ex)(HANDLE,HANDLE , char*,DWORD);
typedef DWORD (WINAPI *getbasename)(HANDLE,HMODULE,char*,DWORD);
static enumproc enum_processes;
static enummod enum_process_modules;
static getfilename_ex getfilenameex;
static getbasename GetModuleBaseNameA;


typedef DWORD (*plist_proc)(void);
DWORD Win95Lister(void);
DWORD NTLister(void);

plist_proc ProcessListFunc;
pslist *processlist;
static unsigned long numprocs, g_dowindows;


extern DWORD gdwPlatform;
extern void make_err_str(unsigned int error,char *buf,int size) ;

BOOL CALLBACK enum_wincb(HWND hwnd,LONG nump) {

	unsigned int i;
	DWORD pid = 0;

	if (!GetWindowThreadProcessId(hwnd,&pid))
		return TRUE;
	
	for (i =0;i < nump;i++) {
		if (processlist[i].pid == pid){
			processlist[i].hwnd = hwnd;
			processlist[i].title[0] =0;
			GetWindowText(hwnd,processlist[i].title,sizeof(processlist[i].title));
			break;
		}
	}
	return TRUE;
}
DWORD Win95Lister(void) {


	HANDLE hsnap;
	PROCESSENTRY32 pe;
	unsigned long nump =0;

//	if (processlist)
//		p_free(processlist);

	hsnap = createSnapshot(TH32CS_SNAPPROCESS,0);

	if (hsnap == INVALID_HANDLE_VALUE)
		return 0;

	pe.dwSize = sizeof(PROCESSENTRY32);
	if (proc32First(hsnap,&pe) ) {
		processlist = p_malloc(100*sizeof(pslist));
		if (!processlist)
			goto done;

		while(1) {
			lstrcpy(processlist[nump].exename,pe.szExeFile);
			processlist[nump].title[0] = 0;
			processlist[nump].pid = pe.th32ProcessID;
			nump++;
			if (!proc32Next(hsnap,&pe))
				break;
		}
	}
done:
	CloseHandle(hsnap);

	if (g_dowindows) {
		EnumWindows(enum_wincb,nump);
	}
	return nump;
}

DWORD NTLister(void) {
	

	DWORD procs[200],dummy,ignore;
	HANDLE hproc;
	HMODULE hmod;
	unsigned int i;
	


	if (!enum_processes(procs,sizeof(procs),&dummy) ) {
		return 0;
	}

	dummy = dummy/sizeof(DWORD); // number of entries filled

//	if (processlist)
//		p_free(processlist);
	processlist = p_malloc(dummy*sizeof(pslist));
	if (!processlist){
		goto done;
	}

	for(i=0 ; i< dummy;i++) {
		processlist[i].pid = procs[i];
		processlist[i].title[0] = 0;
		hproc = OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,
							FALSE,procs[i]);
		if (hproc) {
			if (enum_process_modules(hproc,&hmod,sizeof(hmod),&ignore)) {
				GetModuleBaseNameA(hproc,hmod, processlist[i].exename,MAX_PATH);
			}
			else
				lstrcpy(processlist[i].exename,"(unknown)");
			CloseHandle(hproc);
		}
		else
			lstrcpy(processlist[i].exename,"(unknown)");
		
	}
done:

	if (g_dowindows) {
		EnumWindows(enum_wincb,dummy);
	}
	return dummy;
}
DWORD NTLister_old_style( void) {

	DWORD                        rc;
	HKEY                         hKeyNames;
	DWORD                        dwType;
	DWORD                        dwSize;
	LPBYTE                       buf = NULL;
	char                         szSubKey[1024];
	LANGID                       lid;
	LPSTR                        p;
	LPSTR                        p2;
	PPERF_DATA_BLOCK             pPerf;
	PPERF_OBJECT_TYPE            pObj;
	PPERF_INSTANCE_DEFINITION    pInst;
	PPERF_COUNTER_BLOCK          pCounter;
	PPERF_COUNTER_DEFINITION     pCounterDef;
	DWORD                        i;
	DWORD                        dwProcessIdTitle;
	DWORD                        dwProcessIdCounter;
	char                         szProcessName[MAX_PATH];
	unsigned long numprocesses=0;



	lid = MAKELANGID( LANG_ENGLISH, SUBLANG_NEUTRAL );
	wsprintf( szSubKey, "%s\\%03x", REGKEY_PERF, lid );
	rc = RegOpenKeyEx( HKEY_LOCAL_MACHINE, szSubKey,0,KEY_READ,&hKeyNames);
	if (rc != ERROR_SUCCESS) {
		goto exit;
	}

	rc = RegQueryValueEx( hKeyNames, REGSUBKEY_COUNTERS, NULL, &dwType,
			NULL, &dwSize);

	if (rc != ERROR_SUCCESS) 
		goto exit;

	buf =  p_malloc( dwSize );
	if (buf == NULL) 
		goto exit;

	rc = RegQueryValueEx( hKeyNames, REGSUBKEY_COUNTERS, NULL, &dwType,
			buf, &dwSize);

	if (rc != ERROR_SUCCESS) 
		goto exit;


	p = buf;
	while (*p) {
		if (p > buf) {
			for( p2=p-2; isdigit(*p2); p2--) ;
		}
		if (lstrcmpi(p, PROCESS_COUNTER) == 0) {
			for( p2=p-2; isdigit(*p2); p2--) ;
			lstrcpy( szSubKey, p2+1 );
		}
		else if (lstrcmpi(p, PROCESSID_COUNTER) == 0) {
			for( p2=p-2; isdigit(*p2); p2--) ;
			dwProcessIdTitle = atol( p2+1 );
		}
		p += (lstrlen(p) + 1);
	}

	p_free( buf );


	dwSize = 65535;

	buf = p_malloc( dwSize );
	if (buf == NULL) 
		goto exit;


	while (1) {

		rc = RegQueryValueEx( HKEY_PERFORMANCE_DATA, szSubKey, NULL,
				&dwType, buf, &dwSize);

		pPerf = (PPERF_DATA_BLOCK) buf;

		if ((rc == ERROR_SUCCESS) &&
				(dwSize > 0) &&
				(pPerf)->Signature[0] == (WCHAR)'P' &&
				(pPerf)->Signature[1] == (WCHAR)'E' &&
				(pPerf)->Signature[2] == (WCHAR)'R' &&
				(pPerf)->Signature[3] == (WCHAR)'F' ) {
			break;
		}

		if (rc == ERROR_MORE_DATA) {
			dwSize += 32000;
			buf = p_realloc( buf, dwSize );
		}
		else {
			goto exit;
		}
	}

	//
	// set the perf_object_type pointer
	//
	pObj = (PPERF_OBJECT_TYPE) ((DWORD)pPerf + pPerf->HeaderLength);

	//
	// loop thru the performance counter definition records looking
	// for the process id counter and then save its offset
	//
	pCounterDef = (PPERF_COUNTER_DEFINITION) ((DWORD)pObj + pObj->HeaderLength);
	for (i=0; i<(DWORD)pObj->NumCounters; i++) {
		if (pCounterDef->CounterNameTitleIndex == dwProcessIdTitle) {
			dwProcessIdCounter = pCounterDef->CounterOffset;
			break;
		}
		pCounterDef++;
	}


	pInst = (PPERF_INSTANCE_DEFINITION) ((DWORD)pObj + pObj->DefinitionLength);

	numprocesses = pObj->NumInstances;

//	if (processlist)
//		p_free(processlist);

	processlist = p_malloc(numprocesses*sizeof(pslist));

	if (!processlist){
		goto exit;
	}
	for (i=0; i<numprocesses; i++) {
		//
		// pointer to the process name
		//
		p = (LPSTR) ((DWORD)pInst + pInst->NameOffset);

		//
		// convert it to ascii
		//
		rc = WideCharToMultiByte( CP_ACP,
				0,
				(LPCWSTR)p,
				-1,
				szProcessName,
				sizeof(szProcessName),
				NULL,
				NULL
								);

		if (!rc) {
			lstrcpy( processlist[i].exename, "unknown" );
		}
		if (!lstrcmpi(szProcessName,"_Total")){
			numprocesses--;
			continue;
		}

		if (lstrlen(szProcessName)+4 <= sizeof(processlist[i].exename)) {
			lstrcpy( processlist[i].exename, szProcessName );
			lstrcat( processlist[i].exename, ".exe" );
		}

		pCounter = (PPERF_COUNTER_BLOCK) ((DWORD)pInst + pInst->ByteLength);
		processlist[i].pid = *((LPDWORD) ((DWORD)pCounter + dwProcessIdCounter));
		//        if (processlist[i].pid == 0) {
		//            processlist[i].pid = (DWORD)-2;
		//        }

		pInst = (PPERF_INSTANCE_DEFINITION) 
			((DWORD)pCounter + pCounter->ByteLength);
	}

	if (g_dowindows) {
		EnumWindows(enum_wincb,numprocesses);
	}
exit:
	if (buf) {
		p_free( buf );
	}

	RegCloseKey( hKeyNames );
	RegCloseKey( HKEY_PERFORMANCE_DATA );

	return numprocesses;
}
void init_plister(void) {

	HANDLE hlib;
	//char errbuf[128];

	if (gdwPlatform == VER_PLATFORM_WIN32_NT) {
		hlib = LoadLibrary("psapi.dll");

		if (!hlib){
			ProcessListFunc = NTLister_old_style;
			return;
		}
		enum_processes = (enumproc)GetProcAddress(hlib,"EnumProcesses");
		enum_process_modules = (enummod)GetProcAddress(hlib,
							"EnumProcessModules");
		getfilenameex = (getfilename_ex)GetProcAddress(hlib,
							"GetModuleFileNameExA");
		GetModuleBaseNameA = (getbasename)GetProcAddress(hlib,
							"GetModuleBaseNameA");

		if (!enum_processes || !enum_process_modules || !getfilenameex){
			FreeLibrary(hlib);
			//fprintf(stderr,"Can't find exports in psapi.dll\n");
			return;
		}
		ProcessListFunc = NTLister;
	}
	else{
		hlib = LoadLibrary("kernel32.dll");
		if (!hlib){
			//make_err_str(GetLastError(),errbuf,128);
			//fprintf(stderr,"Can't load kernel32.dll:%s)\n",errbuf);
			return;
		}
		proc32First = (walker)GetProcAddress(hlib,"Process32First");
		proc32Next = (walker)GetProcAddress(hlib,"Process32Next");
		createSnapshot= (create_snapshot)GetProcAddress(hlib,
										"CreateToolhelp32Snapshot");
		if (!proc32First || !proc32Next || !createSnapshot) {
			FreeLibrary(hlib);
			//fprintf(stderr,"Can't find exports in kernel32.dll\n");
			return;
		}
		ProcessListFunc = Win95Lister;
	}

}
int bin_ps (char *name, char **v, char *ops, int func) {
	
	DWORD nump;
	unsigned int i,k;

	if (!ProcessListFunc)
		return 1;
	for (k = 0; v[k] != NULL ; k++){
		if ( v[k][0] == '-' ) {
			if( (v[k][1] == 'W') || (v[k][1] == 'w'))
				g_dowindows = 1;
		}
	}
	nump = ProcessListFunc();

	for(i=0; i< nump; i++) {
		if (gdwPlatform == VER_PLATFORM_WIN32_NT) 
			printf("%u  %-20s %-30s\n",processlist[i].pid,
							processlist[i].exename, 
							g_dowindows?processlist[i].title:"");
		else
			printf("0x%08x  %-20s %-30s\n",processlist[i].pid,
							processlist[i].exename,
							g_dowindows?processlist[i].title:"");
	}
	g_dowindows =0;
	if (processlist)
		p_free(processlist);

	return 0;
}
static char shutdown_usage[]= {"shutdown -[r|l][f] now\n-r reboots, -l logs\
off the current user\n-f forces termination of running applications.\n\
The default action is to shutdown without a reboot.\n\"now\" must be \
specified to actually shutdown or reboot\n"};

int bin_shutdown(char *name, char **v, char *ops, int func){

	unsigned int flags = 0;
	unsigned char reboot,shutdown,logoff,shutdown_ok;
	char *ptr;
	char errbuf[128];
	int k;
	HANDLE hToken;
	TOKEN_PRIVILEGES tp,tpPrevious;
	LUID luid;
	DWORD cbPrevious = sizeof(TOKEN_PRIVILEGES);


	shutdown_ok = reboot = shutdown = logoff = 0;

	if (gdwPlatform != VER_PLATFORM_WIN32_NT) {
		fprintf(stderr,"Sorry, this function is not supported on win95\n");
		return 0;
	}

	for (k = 0; v[k] != NULL ; k++){
		if ( v[k][0] == '-' ) {
			ptr = v[k];
			ptr++;
			while( ptr && *ptr) {
				if (*ptr == 'f')
					flags |= EWX_FORCE;
				if (*ptr == 'r')
					reboot =1;
				else if (*ptr == 'l')
					logoff =1;
				else {
					fprintf(stderr,"Usage: %s",shutdown_usage);
					return 1;
				}
				ptr++;
			}
		}
		else if (!lstrcmpi(v[k],"now")) {
			shutdown_ok = 1;
		}
	}
	if (k == 0) {
		fprintf(stderr,"Usage: %s",shutdown_usage);
		return 1;
	}
	if (!reboot && !logoff){
		flags |= EWX_SHUTDOWN;
		shutdown = 1;
	}
	if (reboot && logoff ) {
		fprintf(stderr,"Usage: %s",shutdown_usage);
		return 1;
	}
	if (reboot)
		flags |= EWX_REBOOT;
	if (logoff)
		flags |= EWX_LOGOFF;

	if ((reboot || shutdown) && (!shutdown_ok) ) {
		fprintf(stderr,"shutdown: Specify \"now\" to really shutdown\n");
		return 1;
	}


	if (!OpenProcessToken(GetCurrentProcess(),
							TOKEN_ADJUST_PRIVILEGES| TOKEN_QUERY,
							&hToken) ){
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"shutdown failed %s\n",errbuf);
		return 1;
	}
							

	if (!LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&luid)) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"shutdown failed %s\n",errbuf);
		return 1;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),&tpPrevious,
				&cbPrevious)){
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"shutdown failed %s\n",errbuf);
		return 1;
	}
	tpPrevious.PrivilegeCount = 1;
	tpPrevious.Privileges[0].Luid = luid;
	tpPrevious.Privileges[0].Attributes |= SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken,FALSE,&tpPrevious,cbPrevious,NULL,
				NULL)){
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"shutdown failed %s\n",errbuf);
		return 1;
	}
	if  (  !ExitWindowsEx(flags,0) ) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"shutdown failed %s\n",errbuf);
		return 1;
	}
	return 1;
}
