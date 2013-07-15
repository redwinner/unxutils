
#include<windows.h>
#include<winioctl.h>
#include<shlobj.h>
#include<direct.h>


int system_error(void) {
	// A function to retrieve, format, and print out a message from the
	// last errror.
	//
	char *ptr = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,                                   // source of message string.
		// ignored for system messages.
		GetLastError(),                 // message number.
		0,                                              // use language neutral message.
		(char *)&ptr,                           // address of pointer to hold output.
		1024,                                   // max size of output.
		NULL);                                  // other params to be inserted in
	// message string.
	
	fprintf(stderr,ptr);
	LocalFree(ptr);

return 0;
}


extern "C" int sync(void)
{

	char devname[MAX_PATH];DWORD ret;
	DWORD bytesreturned;
	char argv[3];

   for( int drive = 3; drive <= 26; drive++ ) 
   {
    sprintf(argv, "%c:", drive + 'A' - 1 );
	
	if (argv[0]=='\\') strcpy(devname,argv);
	else {strcpy(devname,"\\\\.\\");strcat(devname,argv);}

	DWORD dwDesiredAccess=GENERIC_WRITE|GENERIC_READ;
	DWORD fdwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;
	HANDLE handle=CreateFile(devname,dwDesiredAccess,fdwShareMode,NULL,OPEN_EXISTING,0,NULL);
	if(handle==INVALID_HANDLE_VALUE) // CD-ROM?
	{
		dwDesiredAccess=GENERIC_READ;fdwShareMode=FILE_SHARE_READ;
		handle=CreateFile(devname,dwDesiredAccess,fdwShareMode,NULL,OPEN_EXISTING,0,NULL);
//		if (handle==INVALID_HANDLE_VALUE)
//		{fprintf(stderr,"sync failed on %s: ",argv);system_error();}
	}

	if(handle!=INVALID_HANDLE_VALUE)
	{
			ret=FlushFileBuffers(handle); 
			if(!ret) {fprintf(stderr, "sync failed on %s: ",argv);system_error();CloseHandle(handle);}
			CloseHandle(handle);
	}


   }

   return 0;
}