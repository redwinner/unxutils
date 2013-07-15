// walk.c ...
// stolen from one of mstanley's closed SRs by amold
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <imagehlp.h>
#include "ntport.h"

#define xmalloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s))

#define MAXSYMBOLNAMELENGTH 64

#define SYM_HANDLE  GetCurrentProcess()

#if defined(_M_IX86)
#define MACHINE_TYPE  IMAGE_FILE_MACHINE_I386
#elif defined(_M_MRX000)
#define MACHINE_TYPE  IMAGE_FILE_MACHINE_R4000
#elif defined(_M_ALPHA)
#define MACHINE_TYPE  IMAGE_FILE_MACHINE_ALPHA
#elif defined(_M_PPC)
#define MACHINE_TYPE  IMAGE_FILE_MACHINE_POWERPC
#else
#error( "unknown target machine" );
#endif



LONG WINAPI MyExceptionFilter ( EXCEPTION_POINTERS * lpep) {

	BOOL rVal;
	STACKFRAME StackFrame;
	CONTEXT Context;
	IMAGEHLP_SYMBOL *pImagehlpSymbol;
	ULONG Displacement;
	BOOL fReturn;
	CHAR szUndecoratedName[MAXSYMBOLNAMELENGTH];
	FILE * flog;

	SymSetOptions(0);
	SymInitialize(SYM_HANDLE, NULL, TRUE);

	flog = fopen("c:\\Except.log","a");
	if (!flog)
		return EXCEPTION_CONTINUE_SEARCH;
	printf("\ndumping stack trace\n");
	ZeroMemory(&StackFrame, sizeof(StackFrame));
	Context = *lpep->ContextRecord;

#if defined(_M_IX86)
	StackFrame.AddrPC.Offset = Context.Eip;
	StackFrame.AddrPC.Mode = AddrModeFlat;
	StackFrame.AddrFrame.Offset = Context.Ebp;
	StackFrame.AddrFrame.Mode = AddrModeFlat;
	StackFrame.AddrStack.Offset = Context.Esp;
	StackFrame.AddrStack.Mode = AddrModeFlat;
#endif

	pImagehlpSymbol = (IMAGEHLP_SYMBOL *) xmalloc(sizeof(IMAGEHLP_SYMBOL) +
												MAXSYMBOLNAMELENGTH - 1);
	ZeroMemory(pImagehlpSymbol, sizeof(IMAGEHLP_SYMBOL) + MAXSYMBOLNAMELENGTH -
			   1);
	pImagehlpSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
	pImagehlpSymbol->MaxNameLength = MAXSYMBOLNAMELENGTH;

	do {
		rVal = StackWalk ( MACHINE_TYPE,
				SYM_HANDLE,
				0,
				&StackFrame,
				&Context,
				ReadProcessMemory,
				SymFunctionTableAccess,
				SymGetModuleBase,
				NULL);
		if (rVal) {
			pImagehlpSymbol->Address = StackFrame.AddrPC.Offset;
			fReturn = SymGetSymFromAddr ( SYM_HANDLE,
					StackFrame.AddrPC.Offset,
					&Displacement,
					pImagehlpSymbol
				);
			fprintf(flog,"%08x %08x  ", StackFrame.AddrFrame.Offset,
				StackFrame.AddrReturn.Offset);
			printf("%08x %08x  ", StackFrame.AddrFrame.Offset,
				StackFrame.AddrReturn.Offset);
			if (fReturn) {
				fReturn = SymUnDName ( pImagehlpSymbol, szUndecoratedName,		
						 MAXSYMBOLNAMELENGTH);

				if (fReturn) {
					fprintf(flog,"%s", szUndecoratedName);
					printf("%s", szUndecoratedName);
					if (Displacement){
						fprintf(flog,"+%x", Displacement);
						printf("+%x", Displacement);
					}
				}
			} else{
				fprintf(flog,"0x%08x", StackFrame.AddrPC.Offset);
				printf("0x%08x", StackFrame.AddrPC.Offset);
			}
			fprintf(flog,"\n");
			printf("\n");
		}
	}
	while (rVal);

	SymCleanup(SYM_HANDLE);
	fprintf(flog,"----Hit ^c to exit----\n");
	fclose(flog);
	ExitProcess((DWORD)-1);
	return EXCEPTION_CONTINUE_SEARCH;//EXCEPTION_EXECUTE_HANDLER;
}
void init_exceptions(void) {
//	SetUnhandledExceptionFilter(MyExceptionFilter);
}
