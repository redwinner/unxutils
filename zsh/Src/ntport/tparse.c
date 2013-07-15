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
#include <stdio.h>
#include <string.h>
#include <ntport.h>

#define AL 'a'
#define BL 'b'
#define BT 'c'
#define CD 'd'
#define CE 'e'
#define CL 'f'
#define CR '\015'
#define DC 'g'
#define DL 'h'
#define DO 'A'
#define HO 'B'
#define KB '\008'
#define KD 'D'
#define KH 'E'
#define KF 'F'
#define KR 'G'
#define KU 'H'
#define LE 'I'
#define ND 'J'
#define SE 'K'
#define SF '\012'
#define SO 'M'
#define TA '\011'
#define UE 'O'
#define UP 'P'
#define US 'Q'
#define UPPER_LE 'R'
#define UPPER_RI 'S'
#define UPPER_UP 'T'
#define UPPER_DO 'U'
//#define UPPER_DC 'V'
//#define UPPER_IC 'W'

static char mult_buf[6];
static int  curr_pos;
static int state;
#define PARSING_MULT 1

int move_cursor(char,int) ;
extern void NT_MoveToLineOrChar(int,int);
extern void nt_move_next_tab(void);
extern int nt_ClearEOL( void) ;
extern void NT_ClearEOD( void) ;
extern void NT_ClearScreen(void) ;

extern void nt_term_standout_on(void);
extern void nt_term_standout_off(void);
int tc_putc(char c, FILE* outstream) {

	int fd=fileno(outstream);
	int rc=0;
	char *tmp;

	if( (state == PARSING_MULT) &&( c != 'Z') ){
		mult_buf[curr_pos++]=c;
		return 0;
	}
	switch (c) {
		case CD:
			NT_ClearEOD();
			break;
		case CE:
			nt_ClearEOL();
			break;
		case CL:
			NT_ClearScreen();
			break;
		case KB:
			nt_write(fd,&c,1);
			break;
		case LE:
			NT_MoveToLineOrChar(-1,0);
			break;
		case ND:
			NT_MoveToLineOrChar(1,0);
			break;
		case UPPER_RI:
			mult_buf[curr_pos++] = UPPER_RI;
			state = PARSING_MULT;
			break;
		case UPPER_LE:
			mult_buf[curr_pos++] = UPPER_LE;
			state = PARSING_MULT;
			break;
		case UPPER_UP:
			mult_buf[curr_pos++] = UPPER_UP;
			state = PARSING_MULT;
			break;
		case UPPER_DO:
			mult_buf[curr_pos++] = UPPER_DO;
			state = PARSING_MULT;
			break;
		case DO:
			NT_MoveToLineOrChar(1,1);
			break;
		case KU:
			NT_MoveToLineOrChar(-1,1);
			break;
		case UP:
			NT_MoveToLineOrChar(-1,1);
			break;
		case TA:
			nt_move_next_tab();
			break;
		case SO:
			nt_term_standout_on();
			break;
		case SE:
			nt_term_standout_off();
			break;
		case UE:
			break;
		case US:
			break;
		case 'Z':
			tmp = &mult_buf[1];
			mult_buf[curr_pos]=0;
			rc = atoi(tmp);
			move_cursor(mult_buf[0],rc);
			rc = 0;
			curr_pos=0;
			state = 0;
			break;
		default:
			nt_write(fd,&c,1);
			break;
	}
	return rc;
}
int move_cursor(char c,int howmany) {
	switch (c) {
		case  UPPER_UP: //up
			NT_MoveToLineOrChar(-howmany,1);
			break;
		case  UPPER_DO: //down
			NT_MoveToLineOrChar(howmany,1);
			break;
		case  UPPER_RI: //right
			NT_MoveToLineOrChar(howmany,0);
			break;
		case  UPPER_LE: //left
			NT_MoveToLineOrChar(-howmany,0);
			break;
		default :
			break;
	}
	return 0;
}
