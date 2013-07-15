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
// io.c
// wrapper functions for i/o.
//
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <memory.h>
#include <errno.h>
#include "ntport.h"
#include "signal.h"

#define CR 0x0d

extern long lines, columns;
extern void make_err_str(unsigned int ,char *,int ) ;
extern void generic_handler(int);
int consoleread(HANDLE , unsigned char * ,int ) ;

extern int f_isset(int,int);

#define xmalloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s))
#define xfree(p) HeapFree(GetProcessHeap(),0,(p))

INPUT_RECORD girec[2048];
HANDLE __h_con_alarm=0;
HANDLE __h_con_int=0;

unsigned short __nt_want_vcode;

static unsigned char *auxbuf;
static int auxsize;

/* 
 * force_read: Forces a ReadFile, instead of ReadConsole 
 *
 */
int force_read(int fd, unsigned char * buf, int howmany) {
	int numread=0,err=0;
	HANDLE hRead ;
	hRead= (HANDLE)_get_osfhandle(fd);
	if (hRead == INVALID_HANDLE_VALUE) {
		return 0;
	}
again:
	if (!ReadFile(hRead, buf,howmany,&numread, NULL ) ){
		err = GetLastError();
		switch(err) {
			case ERROR_IO_PENDING:
				break;
			case ERROR_ACCESS_DENIED:
			case ERROR_INVALID_HANDLE:
				errno = EBADF;
				return -1;
				break;
			case ERROR_HANDLE_EOF:
			case ERROR_BROKEN_PIPE:
				errno = 0;
				return 0;
			default:
				errno = EBADF;
				return 0;
		}
	}
	if (numread == 1 && buf[0] == CR)
		goto again;
	return numread;
}
int nt_read(int fd, unsigned char * buf, int howmany) {

	int numread=0,err=0,i,actual;
	HANDLE hRead ;
	DWORD ftype;
	//
	hRead= (HANDLE)_get_osfhandle(fd);
	if (hRead == INVALID_HANDLE_VALUE)
		hRead = (HANDLE)fd;
	
	ftype = GetFileType(hRead);


	if (ftype == FILE_TYPE_CHAR){
		numread= consoleread(hRead,buf,howmany);
		return numread;
	}
again:
	if (!ReadFile(hRead, buf,howmany,&numread, NULL ) ){
		err = GetLastError();
		switch(err) {
			case ERROR_IO_PENDING:
				break;
			case ERROR_ACCESS_DENIED:
			case ERROR_INVALID_HANDLE:
				errno = EBADF;
				return -1;
				break;
			case ERROR_HANDLE_EOF:
			case ERROR_BROKEN_PIPE:
				errno = 0;
				return 0;
			default:
				errno = EBADF;
				return 0;
		}
	}
	if (!numread)
		return numread;
	if (!auxbuf || !auxsize){
		auxsize = 1024;
		auxbuf = xmalloc(auxsize);
	}
	if (numread > auxsize) {
		xfree(auxbuf);
		auxsize = numread;
		auxbuf = xmalloc(auxsize);

	}
	actual= 0;
	for(i=0;i<numread;i++) {
		if(buf[i] != CR){
			auxbuf[actual]=buf[i];
			actual++;
		}

	}
	if (actual !=0 ){
		if (i >1)
			memcpy(buf,auxbuf,actual);
	}
	else {
		goto again;
	}
	if (howmany > actual) {
		buf[actual]= '\n';
		actual++;
	}
	while(actual < howmany)
		buf[actual++] = '\n';
	return actual;
}

int nt_write(int fd, unsigned char * buf, int howmany) {

	int bytes_rtn,err;//,bwrote;


	fd = (int)_get_osfhandle(fd);
	if (f_isset(fd,0x01) ) // MY_O_APPEND
		SetFilePointer((HANDLE)fd,0,0,FILE_END);
	if(!WriteFile((HANDLE)fd, buf,howmany,(ULONG*)&bytes_rtn,
			NULL)){
		err = GetLastError();
		switch(err) {
			case ERROR_ACCESS_DENIED:
			case ERROR_INVALID_HANDLE:
				errno = EBADF;
				return -1;
				break;
			case ERROR_BROKEN_PIPE:
				errno = EPIPE;
				return -1;
			default:
				errno = EBADF;
				return -1;
		}
		
	}
	return bytes_rtn?bytes_rtn:-1;

}

int consoleread(HANDLE hInput, unsigned char * buf,int howmany) {

	INPUT_RECORD *irec;
	DWORD numread,controlkey,i;
	WORD vcode;
	unsigned char ch;
	int rc, where=0;
	int alt_pressed = 0,memfree=0;
	HANDLE hevents[3];

// This function is called very frequently. So, we don't:
// 1. Declare large arrays on the stack (use girec)
// 2. Allocate any memory unless we really need to.
//
// This gives me the illusion of speedups, so there.
//
// -amol
//
	if (howmany >0) {
		if (howmany > 2048){
			irec = fmalloc(howmany*sizeof(INPUT_RECORD));
			memfree=1;
		}
		else
			irec = &(girec[0]);
		if (!irec){
			errno = ENOMEM;
			return -1;
		}
	}
	while(1) {
		hevents[0] = __h_con_alarm;
		hevents[1] = __h_con_int;
		hevents[2] = hInput;
		rc = WaitForMultipleObjects(3,hevents,FALSE,INFINITE);
		if (rc == WAIT_OBJECT_0) {
			generic_handler(SIGALRM);
		}
		if (rc == (WAIT_OBJECT_0 +1) ){
			errno = EINTR;
			return -1;
		}
		rc = ReadConsoleInput(hInput,irec,howmany,&numread);
		if (!rc) {
			rc = GetLastError();
			switch (rc) {
				case ERROR_INVALID_HANDLE:
				case ERROR_ACCESS_DENIED:
					errno = EBADF;
					break;
			}
			if (memfree)
				ffree(irec);
			return -1;
		}
		for(i=0;i<numread;i++) {
			switch(irec[i].EventType) {
				case WINDOW_BUFFER_SIZE_EVENT:
					columns=irec[i].Event.WindowBufferSizeEvent.dwSize.X;
					lines = irec[i].Event.WindowBufferSizeEvent.dwSize.Y;
					break;
				case KEY_EVENT:
					if (irec[i].Event.KeyEvent.bKeyDown) {
						vcode=(irec[i].Event.KeyEvent.wVirtualKeyCode);
						ch=(irec[i].Event.KeyEvent.uChar.AsciiChar);
						controlkey=(irec[i].Event.KeyEvent.dwControlKeyState);
						if (controlkey & LEFT_ALT_PRESSED)
							alt_pressed=1;
						else if (controlkey & RIGHT_ALT_PRESSED)
							alt_pressed=2;

						/* This hack for arrow keys -amol 9/28/96 */

						if ( !(__nt_want_vcode & 0x01 ))
							goto skippy;
						if (vcode>= VK_F1 && vcode <= VK_F24){
							buf[where++]='\033';
							__nt_want_vcode =vcode - VK_F1 ;
							__nt_want_vcode <<= 8;
							__nt_want_vcode |= 2;
							return 1;
						}
						else if (vcode>= VK_PRIOR && vcode <= VK_DOWN) {
							buf[where++]='\033';
							__nt_want_vcode=24+(vcode -VK_PRIOR );	
							__nt_want_vcode <<= 8;
							__nt_want_vcode |= 2;
							return 1;
						}
						else if (vcode == VK_INSERT) {
							buf[where++]='\033';
							__nt_want_vcode=24+8; 
							__nt_want_vcode <<= 8;
							__nt_want_vcode |= 2;
							return 1;
						}
						else if (vcode == VK_DELETE) {
							buf[where++]='\033';
							__nt_want_vcode=24+9; 
							__nt_want_vcode <<= 8;
							__nt_want_vcode |= 2;
							return 1;
						}
skippy:
						switch(vcode) {
							case VK_ESCAPE:
								buf[where++]='\033';
								break;
							default:
								if(ch){
									if(1==alt_pressed){
										ch += 128;
									}
									/*
									if(2==alt_pressed){
									}
									*/
								}
								if (ch) {
									if (ch == '\r'){
										ch = '\n';
										if ((gdwPlatform == 
												VER_PLATFORM_WIN32_WINDOWS)
											 && ((gosver.dwMinorVersion < 10)
											 || (gisWin95 != 0) ) ){
											DWORD bread;
											ReadFile(hInput,&ch,1,&bread,NULL);
										}
									}
									buf[where++]=ch;
								}
								break;
						}
						alt_pressed=0;
					}
					break;
				default:
					break;
			}
		}
		if (where == 0)
			continue;
		if (where < howmany)
			buf[where]=0;
		break;
	}
	if (memfree)
		ffree(irec);
	if (!where)
		return -1;
	return (where );
}

//
// replacement for creat that makes handle non-inheritable. 
// -amol 
//
int nt_creat(char *filename, int mode) {

	// ignore the bloody mode

	int fd;
	HANDLE retval;
	SECURITY_ATTRIBUTES security;

	security.nLength = sizeof(security);
	security.lpSecurityDescriptor = NULL;
	security.bInheritHandle = FALSE;

	retval = CreateFile(filename,
						GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						&security,
						CREATE_ALWAYS,
						0,
						NULL);

	if (retval == INVALID_HANDLE_VALUE) {
		errno = EACCES;
		return -1;
	}
	fd = _open_osfhandle((long)retval,_O_BINARY);
	if (fd <0) {
		CloseHandle(retval);
		return -1;
	}
	return fd;
	
}
int nt_open(char *filename, int perms,...) { 

	// ignore the bloody mode

	int fd,mode;
	HANDLE retval;
	SECURITY_ATTRIBUTES security;
	DWORD dwAccess, dwSharemode, dwCreateDist;
	va_list ap;

	va_start(ap,perms);
	mode = va_arg(ap,int);
	va_end(ap);

	if (!lstrcmp(filename,"/dev/tty") )
		filename = "CONIN$";
	if (!lstrcmp(filename,"/dev/null") )
		filename = "NUL";
	security.nLength = sizeof(security);
	security.lpSecurityDescriptor = NULL;
	security.bInheritHandle = FALSE;

	switch (perms & (_O_RDONLY | _O_WRONLY | _O_RDWR) ) {
		case _O_RDONLY:
			dwAccess = GENERIC_READ;
			break;
		case _O_WRONLY:
			dwAccess = GENERIC_WRITE;
			break;
		case _O_RDWR:
			dwAccess = GENERIC_READ | GENERIC_WRITE ;
			break;
		default:
			errno = EINVAL;
			return -1;
	}
	switch (perms & (_O_CREAT | _O_TRUNC) ){
		case 0:
			dwCreateDist = OPEN_EXISTING;
			break;
		case _O_CREAT:
			if (perms & _O_EXCL)
				dwCreateDist = CREATE_NEW;
			else
				dwCreateDist = OPEN_ALWAYS;
			break;
		case _O_CREAT | _O_TRUNC:
			dwCreateDist = CREATE_ALWAYS;
			break;
		case _O_TRUNC:
			dwCreateDist = TRUNCATE_EXISTING;
			break;
		default:
			errno = EINVAL;
			return -1;
	}
	USE(dwSharemode);
	retval = CreateFile(filename,
						dwAccess,//GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						&security,
						dwCreateDist,//CREATE_ALWAYS,
						0,
						NULL);

	if (retval == INVALID_HANDLE_VALUE) {
		int err = GetLastError();
		if (err == 2)
			errno = ENOENT;
		else 
			errno = EACCES;
		return -1;
	}
	if (perms & _O_APPEND)
		SetFilePointer(retval,0,0,FILE_END);

	fd = _open_osfhandle((long)retval,_O_BINARY | perms);
	if (fd <0) {
		CloseHandle(retval);
		return -1;
	}
	return fd;
	
}
