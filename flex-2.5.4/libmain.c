/* libmain - flex run-time support library "main" function */

/* $Header: /cvsroot/unxutils/unxutils/flex-2.5.4/libmain.c,v 1.1.1.1 2000/08/16 00:19:46 syring Exp $ */

extern int yylex();

int main( argc, argv )
int argc;
char *argv[];
	{
	while ( yylex() != 0 )
		;

	return 0;
	}
