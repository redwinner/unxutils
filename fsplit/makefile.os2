#	@(#)Makefile	8.1 (Berkeley) 6/6/93
# updated for OS/2 2.x by S. A. Deutscher, 21-Apr-1996

PROG	= fsplit
CC	= gcc
CFLAGS	= -c -O -s
LDFLAGS	= -O -s


fsplit.exe : fsplit.o
	$(CC) $(LDFLAGS) fsplit.o -o fsplit.exe

fsplit.o : fsplit.c
	$(CC) $(CFLAGS) fsplit.c -o fsplit.o

fsplit	: fsplit.exe


.PHONY : clean allclean

clean :
	rm fsplit.o

xclean :
	rm fsplit.exe

allclean : xclean clean
	
