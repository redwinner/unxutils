// Copyright(c) 1997 Amol Deshpande
// amold@microsoft.com
// Redistribution in source or binary form is permitted as long as you 
// retain this notice in the distribution
//
//
// There is one restriction that I impose on any users of this code. If you
// use it in your own application, your application *must* be freely 
// redistributable in source form also.
//
// You are specifically prohibited from enhancing or fixing bugs in this 
// implementation and selling the resultant product. If you make any changes
// that fix bugs in this code, you *must* send me a copy.
//
// I retain all rights to this software, except for the tcsh code.
//
// In no event shall I be liable to any party for direct, indirect, special, 
// incidental, or consequential  damages arising out of the use of this 
// software and its documentation, even if I have been advised of
// the possibility of such damage.
//
// I specifically disclaim any warranties, including, but not limited to, 
// the implied warranties of merchantability and fitness for a particular 
// purpose.  The software provided hereunder is on an "as is" basis, and I
// have no obligation to provide maintenance, support, updates, enhancements, 
// or modifications.
//
// And finally,
// Microsoft Corporation has nothing to do with this code. 
//
// dirent.h
// directory interface functions. Sort of like dirent functions on unix.
//
//
#ifndef DIRENT_H
#define DIRENT_H

//#define _WINSOCKAPI_ // conflicts with timeval
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define NAME_MAX MAX_PATH

#define IS_ROOT 0x01
#define IS_NET  0x02

#define d_fileno d_ino
struct dirent {
	long            d_ino;
	int             d_off;
	unsigned short  d_reclen;
	char            d_name[NAME_MAX+1];
};

typedef struct {
	HANDLE dd_fd;
	int dd_loc;
	int dd_size;
	int flags;
	char orig_dir_name[NAME_MAX +1];
	struct dirent *dd_buf;
}DIR;

DIR *opendir(char*);
struct dirent *readdir(DIR*);
int closedir(DIR*);
void rewinddir(DIR*);
#endif DIRENT_H
