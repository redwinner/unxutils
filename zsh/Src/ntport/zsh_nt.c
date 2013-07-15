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
#include "zsh.h"

char ** semicolonsplit(char *, int );
void semicolon_arrfixenv(char *, char **);

/**/
char *
semicolonarrgetfn(Param pm)
{
    return zjoin(*(char ***)pm->data, ';');
}

/**/
void
semicolonarrsetfn(Param pm, char *x)
{
    char ***dptr = (char ***)pm->data;

    freearray(*dptr);
    *dptr = x ? semicolonsplit(x, pm->flags & PM_UNIQUE) : mkarray(NULL);
    if (pm->ename)
	semicolon_arrfixenv(pm->nam, *dptr);
}
/**/
char **
semicolonsplit(char *s, int uniq)
{
    int ct;
    char *t, **ret, **ptr, **p;

    for (t = s, ct = 0; *t; t++) /* count number of semicolons */
	if (*t == ';')
	    ct++;
    ptr = ret = (char **) zalloc(sizeof(char **) * (ct + 2));

    t = s;
    do {
	s = t;
        /* move t to point at next colon */
	for (; *t && *t != ';'; t++);
	if (uniq)
	    for (p = ret; p < ptr; p++)
		if (strlen(*p) == t - s && ! strncmp(*p, s, t - s))
		    goto cont;
	*ptr = (char *) zalloc((t - s) + 1);
	ztrncpy(*ptr++, s, t - s);
      cont: ;
    }
    while (*t++);
    *ptr = NULL;
    return ret;
}
/**/
void
semicolon_arrfixenv(char *s, char **t)
{
    char **ep, *u;
    int len_s;
    Param pm;

    MUSTUSEHEAP("semicolon_arrfixenv");
    if (t == path)
	cmdnamtab->emptytable(cmdnamtab);
    u = zjoin(t, ';');
    len_s = strlen(s);
    pm = (Param) paramtab->getnode(paramtab, s);
    for (ep = environ; *ep; ep++)
	if (!strncmp(*ep, s, len_s) && (*ep)[len_s] == '=') {
	    pm->env = replenv(*ep, u);
//		(void)SetEnvironmentVariable(*ep,u);
	    return;
	}
    if (isset(ALLEXPORT))
	pm->flags |= PM_EXPORTED;
    if (pm->flags & PM_EXPORTED) {
		pm->env = addenv(s, u);
		(void)SetEnvironmentVariable(s,u);
	}
}
