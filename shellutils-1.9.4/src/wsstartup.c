/* This file is intentionally not left blank.  */

#include <windows.h>
#include <winsock.h>	
#include <stdlib.h>	
#include <stdio.h>	

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

int wsock_startup()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err; 

	wVersionRequested = MAKEWORD( 2, 2 ); 

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		fprintf(stderr,"WinSock: initilization failed!\n");
		exit(2);
	} 
	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */ 
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) 
		{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
//		WSACleanup( );
//		return -2; 
		}

#ifdef DEBUG
   if (opt.debug)
	   fprintf(stderr,"Winsock-Version: %u.%u\n",
		   LOBYTE( wsaData.wVersion ), HIBYTE( wsaData.wVersion ));
#endif

return (int)wsaData.wVersion;
}
