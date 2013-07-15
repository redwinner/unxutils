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

#include "ntport.h"

extern unsigned long bookend1,bookend2;
extern char **environ;

//char ** __saved_environ=0;
#ifdef NTDBG
#undef dprintf
void
dprintf(char *format, ...)
{				/* } */
	va_list vl;
	char putbuf[2048];
	{
		va_start(vl, format);
		vsprintf(putbuf, format, vl);//melkov@cs.muh.ru
		va_end(vl);
		OutputDebugString(putbuf);
	}
}
#endif

int fork_copy_user_mem(HANDLE hproc) {
	
	int bytes,rc;
	int size;

	size =(char*)&bookend2 - (char*)&bookend1;
	//dprintf("hproc 0x%08x, size %u\n",hproc,size);
	rc =WriteProcessMemory(hproc,&bookend1,&bookend1,
					size,
					&bytes);

	if (!rc) {
		__asm { int 3 };
		rc = GetLastError();
		return -1;
	}
	if (size != bytes) {
		dprintf("size %d , wrote %d\n",size,bytes);
	}
	/*
	__saved_environ=environ;
	rc =WriteProcessMemory(hproc,&__saved_environ,&__saved_environ,4, &bytes);

	if (!rc) {
		rc = GetLastError();
		return -1;
	}
	if (4 != bytes) {
		dprintf("size %d , wrote %d\n",size,bytes);
	}
	*/
	return 0;
}
/*
How To Determine Whether an Application is Console or GUI     [win32sdk]
ID: Q90493     CREATED: 15-OCT-1992   MODIFIED: 16-DEC-1996
*/
#include <winnt.h>
#define xmalloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s))
#define xfree(p) HeapFree(GetProcessHeap(),0,(p))
#define XFER_BUFFER_SIZE 2048

int is_gui(char *exename) {

	HANDLE hImage;

	DWORD  bytes;
	DWORD  SectionOffset;
	DWORD  CoffHeaderOffset;
	DWORD  MoreDosHeader[16];

	ULONG  ntSignature;

	IMAGE_DOS_HEADER      image_dos_header;
	IMAGE_FILE_HEADER     image_file_header;
	IMAGE_OPTIONAL_HEADER image_optional_header;


	hImage = CreateFile(exename, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hImage) {
		return 0;
	}

	/*
	 *  Read the MS-DOS image header.
	 */
	if (!ReadFile(hImage, &image_dos_header, sizeof(IMAGE_DOS_HEADER),
			&bytes,NULL)){
		CloseHandle(hImage);
		return 0;
	}

	if (IMAGE_DOS_SIGNATURE != image_dos_header.e_magic) {
		CloseHandle(hImage);
		return 0;
	}

	/*
	 *  Read more MS-DOS header.       */
	if (!ReadFile(hImage, MoreDosHeader, sizeof(MoreDosHeader),
			&bytes,NULL)){
		CloseHandle(hImage);
		return 0;
	}

	/*
	 *  Get actual COFF header.
	 */
	CoffHeaderOffset = SetFilePointer(hImage, image_dos_header.e_lfanew,
			NULL,FILE_BEGIN);

	if (CoffHeaderOffset == (DWORD) -1){
		CloseHandle(hImage);
		return 0;
	}

	CoffHeaderOffset += sizeof(ULONG);

	if (!ReadFile (hImage, &ntSignature, sizeof(ULONG),
			&bytes,NULL)){
		CloseHandle(hImage);
		return 0;
	}

	if (IMAGE_NT_SIGNATURE != ntSignature) {
		CloseHandle(hImage);
		return 0;
	}

	SectionOffset = CoffHeaderOffset + IMAGE_SIZEOF_FILE_HEADER +
		IMAGE_SIZEOF_NT_OPTIONAL_HEADER;

	if (!ReadFile(hImage, &image_file_header, IMAGE_SIZEOF_FILE_HEADER,
			&bytes, NULL)){
		CloseHandle(hImage);
		return 0;
	}

	/*
	 *  Read optional header.
	 */
	if (!ReadFile(hImage, &image_optional_header, 
			IMAGE_SIZEOF_NT_OPTIONAL_HEADER,&bytes,NULL)) {
		CloseHandle(hImage);
		return 0;
	}

	CloseHandle(hImage);

	if (image_optional_header.Subsystem ==IMAGE_SUBSYSTEM_WINDOWS_GUI)
		return 1;
	return 0;
}
int is_9x_gui(char *prog) {
	
	char *progpath;
	DWORD dwret;
	char *pathbuf;
	char *pext;
	
	pathbuf=xmalloc(MAX_PATH);

	progpath=xmalloc(MAX_PATH<<1);

	if (GetEnvironmentVariable("PATH",pathbuf,MAX_PATH) ==0) {
		goto failed;
	}
	
	pathbuf[MAX_PATH]=0;

	dwret = SearchPath(pathbuf,prog,".EXE",MAX_PATH<<1,progpath,&pext);

	if ( (dwret == 0) || (dwret > (MAX_PATH<<1) ) )
		goto failed;
	
	dprintf("progpath is %s\n",progpath);
	dwret = is_gui(progpath);

	xfree(pathbuf);
	xfree(progpath);

	return dwret;

failed:
	xfree(pathbuf);
	xfree(progpath);
	return 0;


}
