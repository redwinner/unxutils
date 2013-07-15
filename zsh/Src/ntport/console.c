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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincon.h>
#include <stdio.h>
#include "ntport.h"

void ScrollBuf(HANDLE,CONSOLE_SCREEN_BUFFER_INFO*,int);
void NT_MoveToLineOrChar(int ,int ) ;
void NT_MoveToLineOrCharAbs(int ,int ) ;
void set_attr(int ,int ) ;
void set_console_attributes(int,int);
int nt_scroll_old(int line1 , int line2,int howmany) ;


#define say
#define make_err_str


#define xmalloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s))
#define xfree(p) HeapFree(GetProcessHeap(),0,(p))

WORD gwFg, gwBg, gwIsAlreadyIntense;

char eolbuf[81];

//
// The following are used to optimize some console routines. It avoids having
// to call GetConsoleScreenBufferInfo.
// Seems to have helped the speed a bit. -amol
//
HANDLE ghstdout;
CONSOLE_SCREEN_BUFFER_INFO gscrbuf;
unsigned int oldcp, dontmuckwithmycodepage;
//
// This function is called to set the values for above variables.
//
void redo_console(void) {

	ghstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if(!GetConsoleScreenBufferInfo(ghstdout, &gscrbuf) ) {
		;
	}

	gwFg = gscrbuf.wAttributes & 0x000f;
	gwBg = gscrbuf.wAttributes & 0x00f0;

	if (gscrbuf.wAttributes & FOREGROUND_INTENSITY)
		gwIsAlreadyIntense=1;
}
void nt_term_init() {

	DWORD dwmode;
	char e[80];
	HANDLE hinput =GetStdHandle(STD_INPUT_HANDLE);

	if (GetEnvironmentVariable("ZSH_DONTSETCP",e,80))
		dontmuckwithmycodepage = 1;
	if (!dontmuckwithmycodepage) {
		oldcp = GetConsoleCP();
		SetConsoleCP(1252);
		SetConsoleOutputCP(1252);
	}
	if (!GetConsoleMode(hinput,&dwmode) ){
		;
	}
	if(!SetConsoleMode(hinput,dwmode | ENABLE_WINDOW_INPUT) ){
		return;
	}
		
	memset(eolbuf,' ',81);

	redo_console();

	return;
}
void set_cooked_and_flags(STARTUPINFO *si) {
	DWORD dwmode;
	HANDLE hinput =(HANDLE)_get_osfhandle(0);

	if (!GetConsoleMode(hinput,&dwmode) ){
		return ;
	}
	if(!SetConsoleMode(hinput,dwmode | ((
				ENABLE_LINE_INPUT |ENABLE_ECHO_INPUT 
				| ENABLE_PROCESSED_INPUT)& ~ENABLE_WINDOW_INPUT)
				) ){
		return;
	}
	DuplicateHandle(GetCurrentProcess(),hinput,
		GetCurrentProcess(),&(si->hStdInput),0,TRUE,DUPLICATE_SAME_ACCESS);
	hinput = (HANDLE)_get_osfhandle(1);
	DuplicateHandle(GetCurrentProcess(),hinput,
		GetCurrentProcess(),&(si->hStdOutput),0,TRUE,DUPLICATE_SAME_ACCESS);
	hinput = (HANDLE)_get_osfhandle(2);
	DuplicateHandle(GetCurrentProcess(),hinput,
		GetCurrentProcess(),&(si->hStdError),0,TRUE,DUPLICATE_SAME_ACCESS);
	si->dwFlags= STARTF_USESTDHANDLES;
	
}
//
// this function is a bit ugly, but I don't know how to do it better
// -amol
//
int nt_ClearEOL( void) {

	CONSOLE_SCREEN_BUFFER_INFO scrbuf;
	HANDLE hStdout =ghstdout ;//GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD numwrote;
	char errbuf[128];
	int num=0;
	COORD savepos;


	if (hStdout == INVALID_HANDLE_VALUE){
		make_err_str(GetLastError(),errbuf,128);
		say("GetStdHandleFailed %s",errbuf);
		ExitProcess(0xFFFF);
	}
	if(!GetConsoleScreenBufferInfo(hStdout, &scrbuf) ) {
		make_err_str(GetLastError(),errbuf,128);
		say("error from Getconsinfo %s",errbuf);
		return 0 ;//ExitProcess(0xFFFF);
	}
	num = scrbuf.srWindow.Right - scrbuf.dwCursorPosition.X +1;

	savepos = scrbuf.dwCursorPosition;

	if (scrbuf.dwCursorPosition.Y == scrbuf.srWindow.Bottom)
		num--;

//	dprintf("eolbuf writing %d\n",num);
	if (!WriteConsole(hStdout,eolbuf,num,&numwrote,NULL) ) {
		fprintf(stderr,"Error %d from writeconsole\n",GetLastError());
		return 0;
	}
	if (!SetConsoleCursorPosition(hStdout, savepos )) {
		fprintf(stderr,"Error %d from setcursor \n",GetLastError());
	}
	//
	// The following, in my considered opinion, was too slow -amol
	//
	/*
	if (!FillConsoleOutputCharacter(hStdout,' ',num,scrbuf.dwCursorPosition,
		&numwrote) ){
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"error from FillCons %s",errbuf);
		return 0 ;//ExitProcess(0xFFFF);
	}
	if (!FillConsoleOutputAttribute(hStdout,scrbuf.wAttributes, num,
		scrbuf.dwCursorPosition,&numwrote)) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"error from FillConsAttr %s",errbuf);
		;
	}
	*/
	return 0;
}
int nt_cursor_right(void) {

	NT_MoveToLineOrChar(1,0);
	return 0;
}
int nt_cursor_left(void) {
	NT_MoveToLineOrChar(-1,0);
	return 0;
}
void nt_move_next_tab(void) {

	CONSOLE_SCREEN_BUFFER_INFO gscrbuf;
	HANDLE hStdout = ghstdout;
	int where;

	if(!GetConsoleScreenBufferInfo(hStdout, &gscrbuf) ) {
		;
	}
	where = 8 - (gscrbuf.dwCursorPosition.X+1)%8;
	gscrbuf.dwCursorPosition.X += where;
	if (!SetConsoleCursorPosition(hStdout, gscrbuf.dwCursorPosition) ) {
		;
	}

}
void NT_MoveToLineOrChar(int where,int line) {

	CONSOLE_SCREEN_BUFFER_INFO gscrbuf;
	HANDLE hStdout = ghstdout;//GetStdHandle(STD_OUTPUT_HANDLE);
	char errbuf[128];


	if (hStdout == INVALID_HANDLE_VALUE){
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"Error from GetStdHandle %s\n",errbuf);
	}
	if(!GetConsoleScreenBufferInfo(hStdout, &gscrbuf) ) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"Error from getconsolebufinfo %s\n",errbuf);
	}
		
	if (line){
	dprintf("cursor position %d, where %d\n",gscrbuf.dwCursorPosition.Y,where);
		if ( ((gscrbuf.dwCursorPosition.Y+where)> (gscrbuf.srWindow.Bottom-1))
			&&( where >0)){
			dprintf("scroll called");
			ScrollBuf(hStdout,&gscrbuf,where);
		}
		else
			gscrbuf.dwCursorPosition.Y += where;
	}
	else
		gscrbuf.dwCursorPosition.X += where;
	if (!SetConsoleCursorPosition(hStdout, gscrbuf.dwCursorPosition) ) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"Error from setcursor %s\n",errbuf);
	}

}
void ScrollBuf(HANDLE hOut, CONSOLE_SCREEN_BUFFER_INFO *scrbuf,int where) {
	
	SMALL_RECT src;
	COORD dest;
	CHAR_INFO chr;
	char errbuf[128];


	src.Left = scrbuf->srWindow.Left;
	src.Top = scrbuf->srWindow.Top+where ;
	src.Right = scrbuf->srWindow.Right;
	src.Bottom = scrbuf->srWindow.Bottom;

	dest.X = scrbuf->srWindow.Left;
	dest.Y = scrbuf->srWindow.Top;

	chr.Char.AsciiChar = ' ';
	chr.Attributes = scrbuf->wAttributes;

	dprintf("this should not be called\n");
	if (!ScrollConsoleScreenBuffer(hOut,
									&src,
									NULL,
									dest,
									&chr) ) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"Error from Scroll %s\n",errbuf);
	}

}
int nt_scroll(int line1 , int line2,int howmany) {

//	CONSOLE_SCREEN_BUFFER_INFO scrbuf;
	HANDLE hStdout = ghstdout;//GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT src;
	COORD dest;
	CHAR_INFO chr;
	char errbuf[128];


	src.Left = gscrbuf.srWindow.Left;
	src.Top =  gscrbuf.srWindow.Top + line1 +howmany ;
	src.Right = gscrbuf.srWindow.Right;
	src.Bottom = gscrbuf.srWindow.Top +line2;


	dest.X = 0;
	dest.Y =  gscrbuf.srWindow.Top + line1;

	chr.Char.AsciiChar = ' ';
	chr.Attributes = gscrbuf.wAttributes;

	if (!ScrollConsoleScreenBuffer(hStdout,
									&src,
									NULL,
									dest,
									&chr) ) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"Error from Scroll %s\n",errbuf);
	}
	return 0;

}
int nt_scroll_old(int line1 , int line2,int howmany) {

	CONSOLE_SCREEN_BUFFER_INFO scrbuf;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	char errbuf[128];


	if(!GetConsoleScreenBufferInfo(hStdout, &scrbuf) ) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"Error from getconsolebufinfo %s\n",errbuf);
		return 1;
	}
	if ( (scrbuf.srWindow.Bottom + howmany ) > scrbuf.dwSize.Y ) {
		return nt_scroll_old(line1, line2, howmany);
	}

	scrbuf.srWindow.Top += howmany;
	scrbuf.srWindow.Bottom += howmany;
	
	if (!SetConsoleWindowInfo(hStdout,TRUE,&(scrbuf.srWindow) ) ) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"Error from Scroll %s\n",errbuf);
	}
	return 0;

}
void NT_MoveToLineOrCharAbs(int where,int line) {

	CONSOLE_SCREEN_BUFFER_INFO scrbuf;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	char errbuf[128];


	if (hStdout == INVALID_HANDLE_VALUE){
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"Error from GetStdHandle %s\n",errbuf);
	}
	/* This gives us the current position, so we only need to change X or
	 Y -amol 12/15/95 */
	if(!GetConsoleScreenBufferInfo(hStdout, &scrbuf) ) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"Error from getconsolebufinfo %s\n",errbuf);
	}
		

	if (line){
		scrbuf.dwCursorPosition.Y = where+scrbuf.srWindow.Top;
	}
	else
		scrbuf.dwCursorPosition.X = where;
	if (!SetConsoleCursorPosition(hStdout, scrbuf.dwCursorPosition) ) {
		make_err_str(GetLastError(),errbuf,128);
		fprintf(stderr,"Error from setcursorabs %s\n",errbuf);
		fprintf(stderr,"tried where %d line %s\n",where,line?"Yes":"No");
	}

}
int nt_getsize(int * lins, int * cols) {
	CONSOLE_SCREEN_BUFFER_INFO scrbuf;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if(!GetConsoleScreenBufferInfo(hStdout, &scrbuf) ) {
		;
	}
	*lins = scrbuf.srWindow.Bottom -scrbuf.srWindow.Top+1 ;
	*cols = scrbuf.srWindow.Right -scrbuf.srWindow.Left +1;
	return 1;
}
void nt_move_cursor(int cursor, int line) {

	extern int li, co;
//	CONSOLE_SCREEN_BUFFER_INFO scrbuf;

	HANDLE hStdout =ghstdout;//GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dest;
	char errbuf[128];


	dest.X = cursor;
	dest.Y = line+gscrbuf.srWindow.Top;
	if (!SetConsoleCursorPosition(hStdout, dest) ) {
		make_err_str(GetLastError(),errbuf,128);
	}
}
void nt_put_cr(void){

	NT_MoveToLineOrCharAbs(0,0);
}
void nt_put_nl(void){
	NT_MoveToLineOrChar(1,1);
}
void NT_ClearEOD(void) {
	CONSOLE_SCREEN_BUFFER_INFO scrbuf;
	DWORD numwrote;
	COORD origin;
	int ht,wt;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hStdout == INVALID_HANDLE_VALUE){
		return ;
	}
	if(!GetConsoleScreenBufferInfo(hStdout, &scrbuf) ) {
		return ;
	}
	origin = scrbuf.dwCursorPosition;
	ht = scrbuf.dwSize.Y - origin.Y;
	wt = scrbuf.dwSize.X - origin.X;
	if(!FillConsoleOutputCharacter(hStdout,' ',ht*wt,origin,&numwrote) ) {
		return ;
	}
	if (!FillConsoleOutputAttribute(hStdout,scrbuf.wAttributes, ht*wt,
		scrbuf.dwCursorPosition,&numwrote)) {
		return;
	}
	return;
}
void NT_ClearScreen(void) {
	CONSOLE_SCREEN_BUFFER_INFO scrbuf;
	DWORD numwrote;
	COORD origin={0,0};
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hStdout == INVALID_HANDLE_VALUE){
		;
	}
	if(!GetConsoleScreenBufferInfo(hStdout, &scrbuf) ) {
		;
	}
	origin.X = scrbuf.srWindow.Left;
	origin.Y = scrbuf.srWindow.Top;
	if(!FillConsoleOutputCharacter(hStdout,' ',scrbuf.dwSize.X*scrbuf.dwSize.Y,
		origin,&numwrote) ) {
		;
	}
	if (!FillConsoleOutputAttribute(hStdout,scrbuf.wAttributes,
		scrbuf.dwSize.X*scrbuf.dwSize.Y,scrbuf.dwCursorPosition,&numwrote)) {
		;
	}
	if (!SetConsoleCursorPosition(hStdout, origin) ) { // home cursor
		;
	}
	return;
}
void NT_ClearScreen_WholeBuffer(void) {
	CONSOLE_SCREEN_BUFFER_INFO scrbuf;
	DWORD numwrote;
	COORD origin={0,0};
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hStdout == INVALID_HANDLE_VALUE){
		;
	}
	if(!GetConsoleScreenBufferInfo(hStdout, &scrbuf) ) {
		;
	}
	if(!FillConsoleOutputCharacter(hStdout,' ',scrbuf.dwSize.X*scrbuf.dwSize.Y,
		origin,&numwrote) ) {
		;
	}
	if (!FillConsoleOutputAttribute(hStdout,scrbuf.wAttributes,
		scrbuf.dwSize.X*scrbuf.dwSize.Y,scrbuf.dwCursorPosition,&numwrote)) {
		;
	}
	if (!SetConsoleCursorPosition(hStdout, origin) ) { // home cursor
		;
	}
	return;
}
void nt_term_standout_on(void) {
	HANDLE outhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attribs = gwFg;

	attribs <<= 4;

	attribs = attribs | (gwBg >> 4);

	SetConsoleTextAttribute(outhandle, attribs);

}
void nt_term_standout_off(void) {
	HANDLE outhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attribs = gwBg;

	attribs = attribs | (gwFg );

	SetConsoleTextAttribute(outhandle, attribs );
}
void nt_term_bold_on(void) {
	set_attr(1,FOREGROUND_INTENSITY);
}
void nt_term_bold_off(void) {
	set_attr(0,FOREGROUND_INTENSITY);
}

void set_attr(int turnon,int attribs) {
	HANDLE outhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD old_attribs;

	CONSOLE_SCREEN_BUFFER_INFO scrbuf;

	if(!GetConsoleScreenBufferInfo(outhandle, &scrbuf) ) {
		return;
	}
	old_attribs = scrbuf.wAttributes;

	if (gwIsAlreadyIntense)
		turnon = 1- turnon;

	if (!turnon)
		old_attribs = old_attribs & ~attribs;
	else
		old_attribs |= attribs;

	SetConsoleTextAttribute(outhandle, old_attribs );
}
#define ANSI_30 0x00
#define ANSI_31 0x04
#define ANSI_32 0x02
#define ANSI_33 0x06
#define ANSI_34 0x01
#define ANSI_35 0x05
#define ANSI_36 0x03
#define ANSI_37 0x07

#define ANSI_40 0x00
#define ANSI_41 0x40
#define ANSI_42 0x20
#define ANSI_43 0x60
#define ANSI_44 0x10
#define ANSI_45 0x50
#define ANSI_46 0x30
#define ANSI_47 0x70

//
// This is a partial, and probably very buggy, implementation of ANSI
// escapes. Have fun fixing it.. -amol
//
int ParseAnsi(char *ptr, FILE* outfile) {

	int len=0,two_d=0;
	char num[]= {0,0,0};
	char *orig = ptr;
	int attr = 0;

	ptr += 2; //skip ESC[
	len +=2;

	do {
		if (*ptr == ';'){
			ptr++;
			len++;
		}
		// Next should be a max of two digits

		if ( (*ptr < '0') || (*ptr > '9') )
			break ;

		// first char is a digit 
		num[0] = *ptr++;

		if ( (*ptr >= '0') && (*ptr <= '9') ) {
			num[1]=*ptr++;
			two_d = 1;
		}

		// Now there must be an 'm'
		//if (*ptr == ';' && num[0] == '1')
		//	continue;

		if (*ptr != 'm' && *ptr != ';' && *ptr != 'C'){
			break;
		}

		
		attr = atoi(num);



		if (*ptr == 'm'){
			ptr++;
			len+= 2;
		}
		else
			len+=1;
		if (two_d){
			len++;
			two_d = 0;
		}
		if (*ptr == 'C') {
			NT_MoveToLineOrChar(attr,0);
			ptr++;
			continue;
		}
		switch (attr) {
			case 0:
				set_console_attributes(-1,0);
				break;	
			case 1:
				set_console_attributes(1,FOREGROUND_INTENSITY);
				break;
			case 30: //black
				set_console_attributes(1,ANSI_30);
				break;
			case 31:
				set_console_attributes(1,ANSI_31);
				break;
			case 32:
				set_console_attributes(1,ANSI_32);
				break;
			case 33: //yellow
				set_console_attributes(1,ANSI_33);
				break;
			case 34: 
				set_console_attributes(1,ANSI_34);
				break;
			case 35:
				set_console_attributes(1,ANSI_35);
				break;
			case 36:
				set_console_attributes(1,ANSI_36);
				break;
			case 37: //white
				set_console_attributes(1,ANSI_37);
				break;
			case 40: //black
				set_console_attributes(0,ANSI_40);
				break;
			case 41:
				set_console_attributes(0,ANSI_41);
				break;
			case 42:
				set_console_attributes(0,ANSI_42);
				break;
			case 43: //yellow ???
				set_console_attributes(0,ANSI_43);
				break;
			case 44: 
				set_console_attributes(0,ANSI_44);
				break;
			case 46:
				set_console_attributes(0,ANSI_45);
				break;
			case 45: 
				set_console_attributes(0,ANSI_46);
				break;
			case 47: //white
				set_console_attributes(0,ANSI_47);
				break;

		}

	}while(*ptr && *ptr == ';');

	while(*ptr) {
		*orig++ = *ptr++;
	}
	*orig = 0;
		
	if (len == 2)
		return 0;
	return len; 
}
//
// Called by ParseAnsi
//
void set_console_attributes(int foreg,int attribs) {
	USHORT attr;
	HANDLE outhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD fg,bg;
	static WORD orig_attribs,prev_attribs;

	CONSOLE_SCREEN_BUFFER_INFO scrbuf;

	if (!orig_attribs) {
		if(!GetConsoleScreenBufferInfo(outhandle, &scrbuf) ) {
			return;
		}
		orig_attribs = scrbuf.wAttributes;
		prev_attribs = orig_attribs;
	}

	attr   = prev_attribs;

	fg = attr & 0x0f;
	bg = attr & 0xf0;

	if (foreg== 1)
		fg = attribs;
	else
		bg = attribs;
	
	attr = bg | fg;

	if (foreg <0) {
		attr = orig_attribs;
		orig_attribs= 0;
	}

	if (!SetConsoleTextAttribute(outhandle, attr )) {
		dprintf("set cons returned %d\n",GetLastError());
	}
	prev_attribs = attr;
}
