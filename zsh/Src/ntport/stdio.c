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
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define STDIO_C
#include <ntport.h>

#define __MAX_OPEN_FILES 64

#define MY_O_APPEND 0x01

extern unsigned long __forked;
typedef struct _myfile {
	unsigned long  handle;
	unsigned long flags;
} MY_FILE;

typedef unsigned long u_long;
#define INVHL (DWORD)(INVALID_HANDLE_VALUE)

MY_FILE __gOpenFiles[__MAX_OPEN_FILES]={0};
MY_FILE __gOpenFilesCopy[__MAX_OPEN_FILES]={0};

MY_FILE *my_stdin=0, *my_stdout=0, *my_stderr=0;


void init_stdio(void) {

	int i;
	__gOpenFiles[0].handle = (u_long)GetStdHandle(STD_INPUT_HANDLE);
	__gOpenFiles[1].handle = (u_long)GetStdHandle(STD_OUTPUT_HANDLE);
	__gOpenFiles[2].handle = (u_long)GetStdHandle(STD_ERROR_HANDLE);

	for(i=3;i<__MAX_OPEN_FILES;i++) {
		__gOpenFiles[i].handle = INVHL;
		__gOpenFilesCopy[i].handle = INVHL;
		__gOpenFiles[i].flags = 0;
	}

	my_stdin = &__gOpenFiles[0];
	my_stdout = &__gOpenFiles[1];
	my_stderr = &__gOpenFiles[2];

}
int f_isset(int fd, int flags) {
	return (__gOpenFiles[fd].flags & flags);
}
int f_hisset(u_long fd, int flags) {
	int i;
	for(i=0;i<__MAX_OPEN_FILES;i++) {
		if (__gOpenFiles[i].handle == fd)
			return (__gOpenFiles[i].flags & flags);
	}
	return 0;
}
void restore_fds(void ) {
	int i;
	__gOpenFiles[0].handle = (u_long)GetStdHandle(STD_INPUT_HANDLE);
	__gOpenFiles[1].handle = (u_long)GetStdHandle(STD_OUTPUT_HANDLE);
	__gOpenFiles[2].handle = (u_long)GetStdHandle(STD_ERROR_HANDLE);
	my_stdin = &__gOpenFiles[0];
	my_stdout = &__gOpenFiles[1];
	my_stderr = &__gOpenFiles[2];
	for(i=3;i<__MAX_OPEN_FILES;i++) {
		if (__gOpenFilesCopy[i].handle == INVHL)
			continue;
		__gOpenFiles[i].handle = __gOpenFilesCopy[i].handle ;
	}
}
void close_copied_fds(void ) {
	int i;
	for(i=3;i<__MAX_OPEN_FILES;i++) {
		if (__gOpenFilesCopy[i].handle == INVHL)
			continue;
		CloseHandle((HANDLE)__gOpenFilesCopy[i].handle);
		__gOpenFilesCopy[i].handle = INVHL;
	}
}
void copy_fds(void ) {
	int i;
	for(i=3;i<__MAX_OPEN_FILES;i++) {
		if (__gOpenFiles[i].handle == INVHL) {
			__gOpenFilesCopy[i].handle = INVHL;
			continue;
		}
		
		if(!DuplicateHandle(GetCurrentProcess(), 
						(HANDLE)__gOpenFiles[i].handle ,
						GetCurrentProcess(), 
						(HANDLE*)&__gOpenFilesCopy[i].handle,
						 0, TRUE, DUPLICATE_SAME_ACCESS) )
			__gOpenFilesCopy[i].handle = INVHL;
	}
}
int nt_clearerr(FILE* ignore) {
	return 0;
}
FILE* nt_fdopen(int fd, char *mode) {
	return (FILE*)(&__gOpenFiles[fd]);
}
int nt_fileno(FILE *stream) {
	int i;

	for(i=0;i<__MAX_OPEN_FILES;i++) {
		if (&__gOpenFiles[i] == (MY_FILE*)stream) {
			return i;
		}
	}
	errno = EBADF;
	return -1;
}
u_long __nt_get_osfhandle(int fd) {
	return (u_long)(__gOpenFiles[fd].handle);
}
int __nt_open_osfhandle(long h1, int mode) {
	int i;

	for(i=0;i<__MAX_OPEN_FILES;i++) {
		if (__gOpenFiles[i].handle == INVHL) {
			if (mode & _O_APPEND)
				__gOpenFiles[i].flags |= MY_O_APPEND;
			__gOpenFiles[i].handle = h1;
			return i;
		}
	}
	errno = EMFILE;
	return -1;
}
int nt_write2(u_long hfile,void *buf,int howmany) {
	
	u_long bytes_rtn,err;

	if (!howmany)
		return 0;

	if (f_hisset(hfile, MY_O_APPEND))
		SetFilePointer((HANDLE)hfile,0,0,FILE_END);

	if (!dontmuckwithmycodepage && 
			(GetFileType((HANDLE)hfile) == FILE_TYPE_CHAR) ) {
		CharToOemBuff(buf,buf,howmany);
	}
	if(!WriteFile((HANDLE)hfile, buf,howmany,&bytes_rtn, NULL)){
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
int nt_fflush(FILE *stream) {
	//
	// no buffering anyway;
	return 0;
}
int nt_fputc(char c, FILE *stream) {

	u_long hwrite;

	hwrite = ((MY_FILE*)stream)->handle;

	if (nt_write2(hwrite,&c,1) < 0)
		return EOF;
	
	return c;

}
int nt_fprintf(FILE *stream,char *format,...) {

	va_list vl;
	u_long hwrite;
	char putbuf[4096];

	__try{
		hwrite = ((MY_FILE*)stream)->handle;
		va_start(vl, format);
		vsprintf(putbuf, format, vl);
		va_end(vl);
	}
	__except(1) {
		errno = EFAULT;
		return -1;
	}
	return nt_write2(hwrite,putbuf,lstrlen(putbuf));

}
int nt_printf(char *format,...) {
	va_list vl;
	u_long hwrite;
	char putbuf[4096];

	__try{
		hwrite = my_stdout->handle;
		va_start(vl, format);
		vsprintf(putbuf, format, vl);
		va_end(vl);
	}
	__except(1) {
		errno = EFAULT;
		return -1;
	}
	return nt_write2(hwrite,putbuf,lstrlen(putbuf));
}
int nt_puts(char * str) {
	char *ptr= str;
	int rc, len = lstrlen(str)+1;
	while(*ptr++)
		;
	ptr[-1] = '\n';
	rc = nt_write2(__gOpenFiles[1].handle,str, len);
	ptr[-1] = '\0';
	return rc;
}
int nt_putchar(char c) {
	return nt_fputc(c,(FILE*)my_stdout);
}
int nt_fclose(FILE *fp) {
	
	CloseHandle((HANDLE)(((MY_FILE*)fp)->handle));
	((MY_FILE*)fp)->handle = INVHL;
	((MY_FILE*)fp)->flags = 0;

	return 0;
}
int nt_close(int fd) {
	
	if (fd == -1)
		return 0;
	CloseHandle((HANDLE)(__gOpenFiles[fd].handle));
	__gOpenFiles[fd].handle = INVHL;
	__gOpenFiles[fd].flags = 0;

	return 0;
}
int nt_fgetc(FILE *instream) {

	unsigned char ch;
	int ret;

	if ( (ret =nt_read(fileno(instream),&ch,1)) <=0)
		return -1;
	return (int)ch;

}
int nt_fputs(char *str, FILE*outstream) {
	
	return nt_write2(((MY_FILE*)outstream)->handle,str,lstrlen(str));
}
int nt_putc(char ch, FILE*outstream) {
	
	return nt_write2(((MY_FILE*)outstream)->handle,(char*)&ch,1);
}

int nt_access(char *filename, int mode) {
	
	DWORD attribs=(DWORD)-1, bintype,lasterror;
	int tries=0;
	char buf[512];

	if (!filename) {
		errno = ENOENT;
		return -1;
	}
	sprintf(buf,"%s",filename);
retry:
	attribs = GetFileAttributes(buf);
	tries++;

	if (attribs == (DWORD) -1) {
		lasterror = GetLastError();
		if( (lasterror == ERROR_FILE_NOT_FOUND) && (mode & X_OK) ) {
			switch(tries){
				case 1:
					sprintf(buf,"%s.exe",filename);
					break;
				case 2:
					sprintf(buf,"%s.cmd",filename);
					break;
				case 3:
					sprintf(buf,"%s.bat",filename);
					break;
				case 4:
					sprintf(buf,"%s.com",filename);
					break;
				default:
					goto giveup;
					break;
			}
			goto retry;
		}
	}
giveup:
	if (attribs == (DWORD)-1 ) {
		if (lasterror == ERROR_FILE_NOT_FOUND ||
					lasterror == ERROR_PATH_NOT_FOUND)
			errno = ENOENT;
		else
			errno = EACCES;
		return -1;
	}
	if ( (mode & W_OK) &&  (attribs & FILE_ATTRIBUTE_READONLY) ) {
		errno = EACCES;
		return -1;
	}
	if (mode & X_OK) {
		if (((mode & XD_OK) == XD_OK) && (attribs & FILE_ATTRIBUTE_DIRECTORY) ){
			errno = EACCES;
			return -1;
		}
		if ((!(attribs & FILE_ATTRIBUTE_DIRECTORY)) && 
				!GetBinaryType(buf,&bintype) &&(tries >4) ) {
			errno = EACCES;
			return -1;
		}
	}
	return 0;
}
HANDLE nt_open2(char *filename, int perms) { 

	HANDLE retval;
	SECURITY_ATTRIBUTES security;
	DWORD dwAccess, dwCreateDist;

	if (!lstrcmp(filename,"/dev/tty") )
		filename = "CONIN$";
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
			return INVALID_HANDLE_VALUE;
	}
	switch (perms & (_O_CREAT | _O_TRUNC) ){
		case 0:
			dwCreateDist = OPEN_EXISTING;
			break;
		case _O_CREAT:
			if (perms & _O_EXCL)
				dwCreateDist = CREATE_NEW;
			else
				dwCreateDist = CREATE_ALWAYS;
			break;
		case _O_CREAT | _O_TRUNC:
			dwCreateDist = CREATE_ALWAYS;
			break;
		case _O_TRUNC:
			dwCreateDist = TRUNCATE_EXISTING;
			break;
		default:
			errno = EINVAL;
			return INVALID_HANDLE_VALUE;
	}
	retval = CreateFile(filename,
						dwAccess,//GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						&security,
						dwCreateDist,//CREATE_ALWAYS,
						0,
						NULL);

	if (retval == INVALID_HANDLE_VALUE) {
		if (GetLastError() == 2)
			errno = ENOENT;
		else
			errno = EACCES;
		return INVALID_HANDLE_VALUE;
	}
	return retval;

}
FILE * nt_fopen(char *file, char *perms) {

	int mode;
	HANDLE h1;

	if (!file || ! perms) {
		errno = EINVAL;
		return NULL;
	}
	switch(*perms) {
		case 'r':
			mode = _O_RDONLY;
			break;
		case 'w':
			mode = _O_WRONLY | _O_CREAT | _O_TRUNC;
			break;
		case 'a':
			mode = _O_WRONLY | _O_CREAT | _O_APPEND;
			break;
		default:
			errno = EINVAL;
		return NULL;
	}
	mode |= _O_BINARY; // text mode is never! needed.

	if (perms[1] && perms[1] == '+' ) {
		mode |= _O_RDWR;
		mode &= (_O_RDONLY | _O_WRONLY);

	}
	h1 = nt_open2(file,mode);
	if (h1 == INVALID_HANDLE_VALUE){
		return NULL;
	}
	for(mode=0;mode<__MAX_OPEN_FILES;mode++) {
		if (__gOpenFiles[mode].handle == INVHL){
			__gOpenFiles[mode].handle = (u_long)h1;
			return (FILE*)(&__gOpenFiles[mode]);
		}
	}
	CloseHandle(h1);
	errno = EMFILE;
	return NULL;
}
int nt_seek(HANDLE h1, long offset, int how) {
	DWORD dwmove;
	int ret;

	switch(how) {
		case SEEK_CUR:
			dwmove = FILE_CURRENT;
			break;
		case SEEK_END:
			dwmove = FILE_END;
			break;
		case SEEK_SET:
			dwmove = FILE_BEGIN;
			break;
		default:
			errno = EINVAL;
			return -1;
	}

	if ((ret=SetFilePointer(h1,offset,NULL,dwmove) )== -1){
		errno = EBADF;
		return -1;
	}
	return ret;
}
int nt_fseek(FILE *stream,long offset, int how) {
	u_long h1 ; 
	h1 =( (MY_FILE*)(stream))->handle;
	return nt_seek((HANDLE)h1,offset,how);
}
int nt_lseek(int fd,long offset, int how) {
	u_long h1 ; 
	h1 =__gOpenFiles[fd].handle;
	return nt_seek((HANDLE)h1,offset,how);
}
long nt_ftell(FILE* stream) {
	u_long h1;
	h1 =( (MY_FILE*)(stream))->handle;

	return (long)SetFilePointer((HANDLE)h1,0,NULL,SEEK_CUR);
}
int nt_isatty(int fd) {
	u_long h1 = __gOpenFiles[fd].handle;
	return (GetFileType((HANDLE)h1) == FILE_TYPE_CHAR);
}
int nt_fread(void *buffer, size_t size, size_t count,FILE*stream) {

	u_long h1;
	int read=0;

	if (size == 0 || count == 0)
		return 0;
	
	h1 = ((MY_FILE*)stream)->handle;

	if (!ReadFile((HANDLE)h1,buffer,size*count,&read,NULL) ) {
		errno = EBADF;
		return -1;
	}
	if (!read)
		return EOF;
	if (read <size)
		abort();

	return read/size;
}
int nt_fwrite(void *buffer, size_t size, size_t count,FILE*stream) {

	u_long h1;
	int wrote=0;

	if (size == 0 || count == 0)
		return 0;
	
	h1 = ((MY_FILE*)stream)->handle;

	if (f_hisset(h1, MY_O_APPEND))
		SetFilePointer((HANDLE)h1,0,0,FILE_END);

	if (!dontmuckwithmycodepage && 
			(GetFileType((HANDLE)h1) == FILE_TYPE_CHAR) ) {
		CharToOemBuff(buffer,buffer,size*count);
	}
	if (!WriteFile((HANDLE)h1,buffer,size*count,&wrote,NULL) ) {
		errno = EBADF;
		return -1;
	}
	if (!wrote){
		errno = EBADF;
		return -1;
	}
	if (wrote <size)
		abort();

	return wrote/size;
}
int nt_dup(int fdin) {

	HANDLE hdup;
	u_long horig =  __gOpenFiles[fdin].handle;
	int ret;
	
	if (!DuplicateHandle(GetCurrentProcess(),
						 (HANDLE)horig,
						 GetCurrentProcess(),
						 &hdup,
						 0,
						 FALSE,
						 DUPLICATE_SAME_ACCESS)) {
		errno = EBADF;
		return -1;
	}
	ret = __nt_open_osfhandle((long)hdup,_O_BINARY | _O_NOINHERIT|
										__gOpenFiles[fdin].flags);

	return  ret;
}
int nt_dup2(int fdorig,int fdcopy) {

	HANDLE hdup;
	u_long horig =  __gOpenFiles[fdorig].handle;
	
	if (__gOpenFiles[fdcopy].handle != INVHL) {
		CloseHandle((HANDLE)__gOpenFiles[fdcopy].handle );
		__gOpenFiles[fdcopy].handle = INVHL;
	}
	if (!DuplicateHandle(GetCurrentProcess(),
						 (HANDLE)horig,
						 GetCurrentProcess(),
						 &hdup,
						 0,
						 fdcopy<3?TRUE:FALSE,
						 DUPLICATE_SAME_ACCESS)) {
		errno = EBADF;
		return -1;
	}
	__gOpenFiles[fdcopy].handle = (u_long)hdup;
	__gOpenFiles[fdcopy].flags =__gOpenFiles[fdorig].flags;

	switch(fdcopy) {
		case 0:
			SetStdHandle(STD_INPUT_HANDLE,hdup);
			break;
		case 1:
			SetStdHandle(STD_OUTPUT_HANDLE,hdup);
			break;
		case 2:
			SetStdHandle(STD_ERROR_HANDLE,hdup);
			break;
	}

	return  0;
}
char *nt_fgets(char *string,int num, FILE* stream) {
	
	u_long bread=0;
	u_long h1;
	int i,j;

	h1 = ((MY_FILE*)stream)->handle;
	if (!ReadFile((HANDLE)h1,string,num-1,&bread,NULL) ) {
		errno = EBADF;
		return NULL;
	}
	if (bread == 0)
		return NULL;
	string[bread]=0;
	for(i=0;i < bread;i++) {
		/* ignore CR */
#ifdef notdef
		if (string[i] == '\r' ) {
			if (string[i+1] == '\n') {
				string[i] = string[i+1];
				string[i+1] = 0;
				i++;break;
			}
			for(j=i;j<read;j++){
				string[j]= string[j+1];
				string[j+1]=0;
			}
		}
#else
		if (0) {
		}
#endif
		else if (string[i] == '\n'){
			i++;
			break;
		}
	}
	j = bread - i; // - 1; // leftover characters
	
	if(SetFilePointer((HANDLE)h1,-j,NULL,SEEK_CUR) <0){
		int err =GetLastError();
	}
	string[i] = 0;

	return string;
}
int nt_pipe2(HANDLE hpipe[2]) {

	SECURITY_ATTRIBUTES secd;

	secd.nLength=sizeof(secd);
	secd.lpSecurityDescriptor=NULL;
	secd.bInheritHandle=FALSE;

	return (!CreatePipe(&hpipe[0],&hpipe[1],&secd,0));
}
int nt_pipe(int hpipe[2]) {
	HANDLE hpipe2[2];

	nt_pipe2(hpipe2);
	hpipe[0] = _open_osfhandle((long)hpipe2[0],O_NOINHERIT);
	hpipe[1] = _open_osfhandle((long)hpipe2[1],O_NOINHERIT);
	return 0;
}
int is_server(char *name) {
	char *p1, *p2;

	p1 = name;
	if ( 
		((p1[0] != '/') && (p1[0] != '\\') ) ||
		((p1[1] != '/') && (p1[1] != '\\') )
	   )
	   	return 0;
	
	p1++;

	p2 = strrchr(name,'/');
	if (!p2)
		p2 = strchr(name,'\\');
/*	if (p2[1] && p2[1] != '*')
		return 0;
	else */{
		p2--;
		while ( (*p2 != '/') && (*p2 != '\\') )
			p2--;
	}
	if (p2 != p1)
		return 0;

	return 1;
}
int nt_stat(char *filename, struct stat *stbuf) {
	
	// stat hangs on server name 
	// Use any  directory, since the info in stat means %$!* on
	// windows anyway.
	// -amol 5/28/97
	if (is_server(filename)) {
		return _stat("C:/",(struct _stat *)stbuf);
	}
	else 
		return _stat(filename,(struct _stat *)stbuf);
}
/*
 * This should be the LAST FUNCTION IN THIS FILE 
 *
 */
#undef fstat
#undef _open_osfhandle
#undef close
int nt_fstat(int fd, struct stat *stbuf) {
	int realfd;
	HANDLE h1;

	errno = EBADF;

	if(!DuplicateHandle(GetCurrentProcess(),
						(HANDLE)__gOpenFiles[fd].handle,
						GetCurrentProcess(),
						&h1,
						0,
						FALSE,
						DUPLICATE_SAME_ACCESS) )
		return -1;
	realfd = _open_osfhandle((long)h1,0);
	if (realfd <0 ) 
		return -1;
	
	if( fstat(realfd,stbuf) <0 ) {
		close(realfd);
		return -1;
	}
	close(realfd);
	errno =0;
	return 0;
	
}

#if 0
int nt_fstat(int fd, struct stat *stbuf) {
	HANDLE h1 = __gOpenFiles[fd].handle;
	BY_HANDLE_FILE_INFORMATION info;

	if(!GetFileInformationByHandle(h1, &info)) {
		errno = EBADF;
		return -1;
	}
	__try {
		stbuf->st_uid = stbuf->st_gid = stbuf->st_ino = 0;
		stbuf->st_nlink = 1;
		if (GetFileType(h1) == FILE_TYPE_CHAR){
			stbuf->dev = stbuf->rdev = h1;
			stbuf->st_mode = S_IFCHR;
		}
		else {
			stbuf->dev = stbuf->rdev = 0;
			stbuf->st_mode = S_IFREG;
		}
		if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			stbuf->st_mode |= S_IFDIR;
		}
		if (info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
			stbuf->st_mode |= S_IFREAD;
		}
		else
			stbuf->st_mode |= S_IFWRITE;

		stbuf->st_size = info.dwFileSizeLow;
	}
	__except(1) {
		errno = EINVAL;
		return -1;
	}
}
#endif 0
