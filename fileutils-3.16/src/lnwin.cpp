#include <windows.h>
#include <stdio.h>

// #define err return doerr( __FILE__, __LINE__ )

int doerr( const char *file, int line )
{
	DWORD e;
	e = GetLastError();
	if ( e == 0 )
		return 1;
	printf( "%s(%d): gle = %lu\n", file, line, e );
	return( 2 );
}

int enableprivs()
{
	HANDLE hToken;
	byte buf[sizeof TOKEN_PRIVILEGES * 2];
	TOKEN_PRIVILEGES & tkp = *( (TOKEN_PRIVILEGES *) buf );

	if ( ! OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		return 1;
	// enable SeBackupPrivilege, SeRestorePrivilege

	if ( !LookupPrivilegeValue( NULL, SE_BACKUP_NAME, &tkp.Privileges[0].Luid ) )
		return 2;
	if ( !LookupPrivilegeValue( NULL, SE_RESTORE_NAME, &tkp.Privileges[1].Luid ) )
		return 3;
	tkp.PrivilegeCount = 2;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tkp.Privileges[1].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp,
		NULL, NULL );
	return 0;
}



extern "C" int link( char* source, char* dest )
{
	HANDLE fh;

	enableprivs(); // in case we aren't admin

	fh = CreateFile( source, GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_POSIX_SEMANTICS, NULL );
	if ( fh == INVALID_HANDLE_VALUE || fh == NULL )
	return 4;

	static char buf1[MAX_PATH];
	static wchar_t buf2[MAX_PATH * 2];
	char *p;
	void *ctx = NULL;
	WIN32_STREAM_ID wsi;
	DWORD numwritten;

	GetFullPathName( dest, MAX_PATH, &buf1[0], &p );

	wsi.dwStreamId = BACKUP_LINK;
	wsi.dwStreamAttributes = 0;
	wsi.dwStreamNameSize = 0;
	wsi.Size.QuadPart = strlen( buf1 ) * 2 + 2;
	MultiByteToWideChar( CP_ACP, 0, buf1, strlen( buf1 ) + 1, buf2, MAX_PATH );

	if ( ! BackupWrite( fh, (byte *) &wsi, 20, &numwritten, FALSE, FALSE, &ctx ) )
		return 5;
	if ( numwritten != 20 )
		return 6;

	if ( ! BackupWrite( fh, (byte *) buf2, wsi.Size.LowPart, &numwritten, FALSE, FALSE, &ctx ) )
		return 7;
	if ( numwritten != wsi.Size.LowPart )
		return 8;

	// make NT release the context
	BackupWrite( fh, (byte *) &buf1[0], 0, &numwritten, TRUE, FALSE, &ctx );

	CloseHandle( fh );

	return 0;
}
