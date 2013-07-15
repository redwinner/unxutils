/*
 * $Id: globals.h,v 2.44 1996/10/15 20:16:35 hzoli Exp $
 *
 * globals.h - global variables for zsh
 *
 * This file is part of zsh, the Z shell.
 *
 * Copyright (c) 1992-1996 Paul Falstad
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and to distribute modified versions of this software for any
 * purpose, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * In no event shall Paul Falstad or the Zsh Development Group be liable
 * to any party for direct, indirect, special, incidental, or consequential
 * damages arising out of the use of this software and its documentation,
 * even if Paul Falstad and the Zsh Development Group have been advised of
 * the possibility of such damage.
 *
 * Paul Falstad and the Zsh Development Group specifically disclaim any
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose.  The software
 * provided hereunder is on an "as is" basis, and Paul Falstad and the
 * Zsh Development Group have no obligation to provide maintenance,
 * support, updates, enhancements, or modifications.
 *
 */


/* GLOBALS is defined is init.c, so the global variables *
 * are actually contained in init.c, and are externs in  *
 * the other source files.                               */

#ifdef GLOBALS
# define EXTERN
#else
# define EXTERN extern
#endif


#if defined (WINNT)
#ifndef GLOBALS
#undef INIT_ZERO
#undef INIT_ZERO_STRUCT
#define INIT_ZERO
#define INIT_ZERO_STRUCT
#endif GLOBALS
#endif WINNT
 
#ifdef GLOBALS
int redirtab[TRINANG - OUTANG + 1] =
{
    WRITE,
    WRITENOW,
    APP,
    APPNOW,
    READ,
    READWRITE,
    HEREDOC,
    HEREDOCDASH,
    MERGEIN,
    MERGEOUT,
    ERRWRITE,
    ERRWRITENOW,
    ERRAPP,
    ERRAPPNOW,
    HERESTR,
};
#else
extern int redirtab[TRINANG - OUTANG + 1];
#endif

#ifdef GLOBALS
char nulstring[] = {Nularg, '\0'};
int  nulstrlen   = sizeof(nulstring) - 1;
#else
extern char nulstring[];
extern int  nulstrlen;
#endif

/* NULL-terminated arrays containing path, cdpath, etc. */
 
EXTERN char **path INIT_ZERO;		/* $path     */
EXTERN char **cdpath INIT_ZERO;		/* $cdpath   */
EXTERN char **fpath INIT_ZERO;		/* $fpath    */
EXTERN char **watch INIT_ZERO;		/* $watch    */
EXTERN char **mailpath INIT_ZERO;		/* $mailpath */
EXTERN char **manpath INIT_ZERO;		/* $manpath  */
EXTERN char **fignore INIT_ZERO;		/* $fignore  */
EXTERN char **psvar INIT_ZERO;		/* $psvar    */
 
EXTERN char *yytext INIT_ZERO;

/* used to suppress ERREXIT and  *
 * trapping of SIGZERR, SIGEXIT. */

EXTERN int noerrexit INIT_ZERO;

/* do not save history on exec and exit */

EXTERN int nohistsave INIT_ZERO;
 
/* error/break flag */
 
EXTERN int errflag INIT_ZERO;
 
/* Status of return from a trap */
 
EXTERN int trapreturn INIT_ZERO;
 
EXTERN char *tokstr INIT_ZERO;
EXTERN int tok, tokfd INIT_ZERO;
 
/* lexical analyzer error flag */
 
EXTERN int lexstop INIT_ZERO;

EXTERN struct heredocs *hdocs INIT_ZERO;
 
/* suppress error messages */
 
EXTERN int noerrs INIT_ZERO;
 
/* nonzero means we are not evaluating, just parsing (in math.c) */
 
EXTERN int noeval INIT_ZERO;
 
/* current history event number */
 
EXTERN int curhist INIT_ZERO;
 
/* if != 0, we are expanding the current line */

EXTERN int expanding INIT_ZERO;

/* these are used to modify the cursor position during expansion */

EXTERN int excs INIT_ZERO, exlast INIT_ZERO;

/* if != 0, this is the first line of the command */
 
EXTERN int isfirstln INIT_ZERO;
 
/* if != 0, this is the first char of the command (not including
        white space) */
 
EXTERN int isfirstch INIT_ZERO;

/* number of history entries */
 
EXTERN int histentct INIT_ZERO;
 
/* array of history entries */
 
EXTERN Histent histentarr INIT_ZERO;
 
/* capacity of history lists */
 
EXTERN int histsiz INIT_ZERO;
 
/* if = 1, we have performed history substitution on the current line
        if = 2, we have used the 'p' modifier */
 
EXTERN int histdone INIT_ZERO;
 
/* default event (usually curhist-1, that is, "!!") */
 
EXTERN int defev INIT_ZERO;
 
/* != 0 if we are about to read a command word */
 
EXTERN int incmdpos INIT_ZERO;
 
/* != 0 if we are in the middle of a [[ ... ]] */
 
EXTERN int incond INIT_ZERO;
 
/* != 0 if we are after a redirection (for ctxtlex only) */
 
EXTERN int inredir INIT_ZERO;
 
/* != 0 if we are about to read a case pattern */
 
EXTERN int incasepat INIT_ZERO;
 
/* != 0 if we just read FUNCTION */
 
EXTERN int infunc INIT_ZERO;
 
/* != 0 if we just read a newline */
 
EXTERN int isnewlin INIT_ZERO;

/* the lists of history events */
 
EXTERN LinkList histlist INIT_ZERO;
 
/* the directory stack */
 
EXTERN LinkList dirstack INIT_ZERO;
 
/* the zle buffer stack */
 
EXTERN LinkList bufstack INIT_ZERO;

/* total # of characters waiting to be read. */

EXTERN int inbufct INIT_ZERO;

/* the flags controlling the input routines in input.c: see INP_* in zsh.h */

EXTERN int inbufflags INIT_ZERO;

/* flag that an alias should be expanded after expansion ending in space */

EXTERN int inalmore;

/* != 0 if this is a subshell */
 
EXTERN int subsh INIT_ZERO;
 
/* # of break levels */
 
EXTERN int breaks INIT_ZERO;
 
/* != 0 if we have a return pending */
 
EXTERN int retflag INIT_ZERO;
 
/* how far we've hashed the PATH so far */
 
EXTERN char **pathchecked INIT_ZERO;
 
/* # of nested loops we are in */
 
EXTERN int loops INIT_ZERO;
 
/* # of continue levels */
 
EXTERN int contflag INIT_ZERO;
 
/* the job we are working on */
 
EXTERN int thisjob INIT_ZERO;

/* the current job (+) */
 
EXTERN int curjob INIT_ZERO;
 
/* the previous job (-) */
 
EXTERN int prevjob INIT_ZERO;
 
/* hash table containing the aliases */
 
EXTERN HashTable aliastab INIT_ZERO;
 
/* hash table containing the reserved words */

EXTERN HashTable reswdtab INIT_ZERO;

/* hash table containing the parameters */
 
EXTERN HashTable paramtab INIT_ZERO;
 
/* hash table containing the external/hashed commands */
 
EXTERN HashTable cmdnamtab INIT_ZERO;

/* hash table containing the shell functions */

EXTERN HashTable shfunctab INIT_ZERO;

/* hash table containing builtin commands */

EXTERN HashTable builtintab INIT_ZERO;
 
/* hash table for completion info for commands */
 
EXTERN HashTable compctltab INIT_ZERO;

/* hash table for multi-character bindings */

EXTERN HashTable keybindtab INIT_ZERO;

/* hash table for emacs multi-character bindings */

EXTERN HashTable emkeybindtab INIT_ZERO;

/* hash table for vi multi-character bindings */

EXTERN HashTable vikeybindtab INIT_ZERO;

/* hash table for named directories */

EXTERN HashTable nameddirtab INIT_ZERO;
 
/* default completion infos */
 
EXTERN struct compctl cc_compos INIT_ZERO_STRUCT, 
			cc_default INIT_ZERO_STRUCT,
			cc_first INIT_ZERO_STRUCT, 
			cc_dummy INIT_ZERO_STRUCT;
 
/* the job table */
 
EXTERN struct job jobtab[MAXJOB]  INIT_ZERO_STRUCT;
 
/* shell timings */
 
EXTERN struct tms shtms INIT_ZERO_STRUCT;
 
/* the list of sched jobs pending */
 
EXTERN struct schedcmd *schedcmds INIT_ZERO;
 
/* the last l for s/l/r/ history substitution */
 
EXTERN char *hsubl INIT_ZERO;

/* the last r for s/l/r/ history substitution */
 
EXTERN char *hsubr INIT_ZERO;
 
/* We cache `USERNAME' and use check cached_uid *
 * so we know when to recompute it.             */

EXTERN uid_t cached_uid INIT_ZERO;
EXTERN char *cached_username INIT_ZERO;   /* $USERNAME   */
EXTERN char *zsh_name INIT_ZERO;		/* ZSH_NAME    */

EXTERN char *underscore INIT_ZERO;	/* $_          */
EXTERN long lastval INIT_ZERO;            /* $?          */
EXTERN long mypid INIT_ZERO;		/* $$          */
EXTERN long lastpid INIT_ZERO;		/* $!          */
EXTERN long ppid INIT_ZERO;		/* $PPID       */
EXTERN char *ifs INIT_ZERO;		/* $IFS        */
EXTERN char *pwd INIT_ZERO;		/* $PWD        */
EXTERN char *oldpwd INIT_ZERO;		/* $OLDPWD     */

EXTERN long columns INIT_ZERO;            /* $COLUMNS    */
EXTERN long lines INIT_ZERO;              /* $LINES      */

EXTERN char *zoptarg INIT_ZERO;		/* $OPTARG     */
EXTERN long zoptind INIT_ZERO;		/* $OPTIND     */
EXTERN char *prompt INIT_ZERO;		/* $PROMPT     */
EXTERN char *prompt2 INIT_ZERO;		/* etc.        */
EXTERN char *prompt3 INIT_ZERO;
EXTERN char *prompt4 INIT_ZERO;
EXTERN char *rprompt INIT_ZERO;		/* $RPROMPT    */
EXTERN char *sprompt INIT_ZERO;

EXTERN char *wordchars INIT_ZERO;
EXTERN char *rstring INIT_ZERO, *Rstring INIT_ZERO;
EXTERN char *postedit INIT_ZERO;

EXTERN char *hostnam INIT_ZERO;           /* from gethostname */
EXTERN char *home INIT_ZERO;              /* $HOME */
EXTERN char **pparams INIT_ZERO;          /* $argv */

EXTERN pid_t mypgrp INIT_ZERO;		/* the process group of the shell */
 
EXTERN char *argzero INIT_ZERO;           /* $0 */
 
EXTERN char *hackzero INIT_ZERO;
EXTERN char *scriptname INIT_ZERO;        /* name of script being sourced */

EXTERN long lineno INIT_ZERO;             /* $LINENO       */
EXTERN long shlvl INIT_ZERO;              /* $SHLVL        */
 
EXTERN long lastval2 INIT_ZERO;

/* the last time we checked mail */
 
EXTERN time_t lastmailcheck INIT_ZERO;
 
/* the last time we checked the people in the WATCH variable */
 
EXTERN time_t lastwatch INIT_ZERO;
 
/* the last time we did the periodic() shell function */
 
EXTERN time_t lastperiodic INIT_ZERO;
 
/* $SECONDS = time(NULL) - shtimer.tv_sec */
 
EXTERN struct timeval shtimer INIT_ZERO_STRUCT;
 
/* the default command for null commands */
 
EXTERN char *nullcmd INIT_ZERO;
EXTERN char *readnullcmd INIT_ZERO;
 
/* the List of local variables we have to destroy */
 
EXTERN LinkList locallist INIT_ZERO;

/* what level of localness we are at */
 
EXTERN int locallevel INIT_ZERO;
 
/* what level of sourcing we are at */
 
EXTERN int sourcelevel INIT_ZERO;

/* The table of file descriptors.  A table element is zero if the  *
 * corresponding fd is not used by the shell.  It is greater than  *
 * 1 if the fd is used by a <(...) or >(...) substitution and 1 if *
 * it is an internal file descriptor which must be closed before   *
 * executing an external command.  The first ten elements of the   *
 * table is not used.  A table element is set by movefd and cleard *
 * by zclose.                                                      */

EXTERN char *fdtable INIT_ZERO;

/* The allocated size of fdtable */

EXTERN int fdtable_size INIT_ZERO;

/* The highest fd that marked with nonzero in fdtable */

EXTERN int max_zsh_fd INIT_ZERO;

/* input fd from the coprocess */

EXTERN int coprocin INIT_ZERO;

/* output fd from the coprocess */

EXTERN int coprocout INIT_ZERO;

/* the shell input fd */

EXTERN int SHIN INIT_ZERO;

/* the shell tty fd */

EXTERN int SHTTY INIT_ZERO;

/* the FILE attached to the shell tty */

EXTERN FILE *shout INIT_ZERO;

/* buffered shell input for non-interactive shells */

EXTERN FILE *bshin INIT_ZERO;

/* != 0 means we are reading input from a string */
 
EXTERN int strin INIT_ZERO;
 
/* != 0 means history substitution is turned off */
 
EXTERN int stophist INIT_ZERO;
 
/* this line began with a space, so junk it if HISTIGNORESPACE is on */
 
EXTERN int spaceflag INIT_ZERO;
 
/* don't do spelling correction */
 
EXTERN int nocorrect INIT_ZERO;

/* state of the history mechanism (see hist.c) */
 
EXTERN int histactive INIT_ZERO;

/* current emulation (used to decide which set of option letters is used) */

EXTERN int emulation INIT_ZERO;
 
/* the options; e.g. if opts[SHGLOB] != 0, SH_GLOB is turned on */
 
EXTERN char opts[OPT_SIZE] INIT_ZERO_STRUCT;
 
EXTERN int lastbase INIT_ZERO;            /* last input base we used */
 
#ifdef HAVE_GETRLIMIT
/* the resource limits for the shell and its children */

EXTERN struct rlimit current_limits[RLIM_NLIMITS];
EXTERN struct rlimit limits[RLIM_NLIMITS];
#endif
 
/* pointer into the history line */
 
EXTERN char *hptr INIT_ZERO;
 
/* the current history line */
 
EXTERN char *chline INIT_ZERO;

/* true if the last character returned by hgetc was an escaped bangchar
 * if it is set and NOBANGHIST is unset hwaddc escapes bangchars */

EXTERN int qbang INIT_ZERO;
 
/* text attribute mask */
 
#ifdef GLOBALS
unsigned txtattrmask = 0;
#else
extern unsigned txtattrmask;
#endif

/* text change - attribute change made by prompts */

EXTERN unsigned txtchange INIT_ZERO;

EXTERN char *term INIT_ZERO;		/* $TERM */
 
/* 0 if this $TERM setup is usable, otherwise it contains TERM_* flags */

EXTERN int termflags INIT_ZERO;
 
/* flag for CSHNULLGLOB */
 
EXTERN int badcshglob INIT_ZERO;
 
/* max size of histline */
 
EXTERN int hlinesz INIT_ZERO;
 
/* we have printed a 'you have stopped (running) jobs.' message */
 
EXTERN int stopmsg INIT_ZERO;
 
/* the default tty state */
 
EXTERN struct ttyinfo shttyinfo INIT_ZERO_STRUCT;
 
EXTERN char *ttystrname INIT_ZERO;	/* $TTY */
 
/* 1 if ttyctl -f has been executed */
 
EXTERN int ttyfrozen INIT_ZERO;
 
/* != 0 if we are allocating in the heaplist */
 
EXTERN int useheap INIT_ZERO;
 
/* Words on the command line, for use in completion */
 
EXTERN int clwsize INIT_ZERO, clwnum INIT_ZERO, clwpos INIT_ZERO;
EXTERN char **clwords INIT_ZERO;

/* pid of process undergoing 'process substitution' */
 
EXTERN pid_t cmdoutpid INIT_ZERO;
 
/* exit status of process undergoing 'process substitution' */
 
EXTERN int cmdoutval INIT_ZERO;
 
/* Stack to save some variables before executing a signal handler function */

EXTERN struct execstack *exstack INIT_ZERO;

/* Array describing the state of each signal: an element contains *
 * 0 for the default action or some ZSIG_* flags ored together.   */

EXTERN int sigtrapped[VSIGCOUNT] INIT_ZERO_STRUCT;

/* trap functions for each signal */

EXTERN List sigfuncs[VSIGCOUNT] INIT_ZERO_STRUCT;

#ifdef DEBUG
EXTERN int alloc_stackp INIT_ZERO;
#endif

/* Variables used by signal queueing */

EXTERN int queueing_enabled INIT_ZERO;
EXTERN sigset_t signal_mask_queue[MAX_QUEUE_SIZE] INIT_ZERO_STRUCT;
EXTERN int signal_queue[MAX_QUEUE_SIZE] INIT_ZERO_STRUCT;
EXTERN int queue_front INIT_ZERO;
EXTERN int queue_rear INIT_ZERO;

/* 1 if aliases should not be expanded */
 
EXTERN int noaliases INIT_ZERO;

#ifdef GLOBALS
/* tokens */
char *ztokens = "#$^*()$=|{}[]`<>?~`,'\"\\";
#else
extern char *ztokens;
#endif

/* $histchars */
 
EXTERN unsigned char bangchar INIT_ZERO, hatchar INIT_ZERO, hashchar INIT_ZERO;
 
EXTERN int eofseen INIT_ZERO;
 
/* we are parsing a line sent to use by the editor */
 
EXTERN int zleparse INIT_ZERO;
 
EXTERN int wordbeg INIT_ZERO;
 
EXTERN int parbegin INIT_ZERO;

EXTERN int parend INIT_ZERO;
 
/* used in arrays of lists instead of NULL pointers */
 
EXTERN struct list dummy_list INIT_ZERO_STRUCT;

/* lengths of each string */
 
EXTERN int tclen[TC_COUNT] INIT_ZERO_STRUCT;
 
EXTERN char *tcstr[TC_COUNT] INIT_ZERO_STRUCT;
 
/* Values of the li and co entries */

EXTERN int tclines INIT_ZERO, tccolumns INIT_ZERO;

/* names of the strings we want */
#ifdef GLOBALS
char *tccapnams[TC_COUNT] =
{
    "cl", "le", "LE", "nd", "RI", "up", "UP", "do",
    "DO", "dc", "DC", "ic", "IC", "cd", "ce", "al", "dl", "ta",
    "md", "so", "us", "me", "se", "ue"
};
#else
extern char *tccapnams[TC_COUNT];
#endif

/* the command stack for use with %_ in prompts */
 
EXTERN unsigned char *cmdstack INIT_ZERO;
EXTERN int cmdsp INIT_ZERO;

#ifdef GLOBALS
char *tokstrings[WHILE + 1] = {
    NULL,	/* NULLTOK	  0  */
    ";",	/* SEPER	     */
    "\\n",	/* NEWLIN	     */
    ";",	/* SEMI		     */
    ";;",	/* DSEMI	     */
    "&",	/* AMPER	  5  */
    "(",	/* INPAR	     */
    ")",	/* OUTPAR	     */
    "||",	/* DBAR		     */
    "&&",	/* DAMPER	     */
    ")",	/* OUTANG	  10 */
    ">|",	/* OUTANGBANG	     */
    ">>",	/* DOUTANG	     */
    ">>|",	/* DOUTANGBANG	     */
    "<",	/* INANG	     */
    "<>",	/* INOUTANG	  15 */
    "<<",	/* DINANG	     */
    "<<-",	/* DINANGDASH	     */
    "<&",	/* INANGAMP	     */
    ">&",	/* OUTANGAMP	     */
    "&>",	/* AMPOUTANG	  20 */
    "&>|",	/* OUTANGAMPBANG     */
    ">>&",	/* DOUTANGAMP	     */
    ">>&|",	/* DOUTANGAMPBANG    */
    "<<<",	/* TRINANG	     */
    "|",	/* BAR		  25 */
    "|&",	/* BARAMP	     */
    "()",	/* INOUTPAR	     */
    "((",	/* DINPAR	     */
    "))",	/* DOUTPAR	     */
    "&|",	/* AMPERBANG	  30 */
};
#else
extern char *tokstrings[];
#endif

#ifdef GLOBALS
char *cmdnames[] =
{
    "for",      "while",     "repeat",    "select",
    "until",    "if",        "then",      "else",
    "elif",     "math",      "cond",      "cmdor",
    "cmdand",   "pipe",      "errpipe",   "foreach",
    "case",     "function",  "subsh",     "cursh",
    "array",    "quote",     "dquote",    "bquote",
    "cmdsubst", "mathsubst", "elif-then", "heredoc",
    "heredocd", "brace",     "braceparam",
};
#else
extern char *cmdnames[];
#endif
 
#ifndef GLOBALS
extern struct option optns[OPT_SIZE];
#else
struct option optns[OPT_SIZE] = {
# define x OPT_REV|
    {NULL, 0, 0, 0},
    {"allexport", 		'a',  'a',  0},
    {"alwayslastprompt", 	0,    0,    0},
    {"alwaystoend", 		0,    0,    0},
    {"appendhistory", 		0,    0,    0},
    {"autocd", 			'J',  0,    0},
    {"autolist", 		'9',  0,    0},
    {"automenu", 		0,    0,    0},
    {"autonamedirs", 		0,    0,    0},
    {"autoparamkeys", 		0,    0,    0},
    {"autoparamslash", 		0,    0,    OPT_CSH},
    {"autopushd", 		'N',  0,    0},
    {"autoremoveslash", 	0,    0,    0},
    {"autoresume", 		'W',  0,    0},
    {"badpattern", 		x'2', 0,    OPT_EMULATE|OPT_NONBOURNE},
    {"banghist", 		x'K', 0,    OPT_EMULATE|OPT_NONBOURNE},
    {"beep", 			x'B', 0,    OPT_ALL},
    {"bgnice", 			'6',  0,    OPT_EMULATE|OPT_NONBOURNE},
    {"braceccl", 		0,    0,    0},
    {"bsdecho", 		0,    0,    OPT_EMULATE|OPT_SH},
    {"cdablevars", 		'T',  0,    0},
    {"chaselinks", 		'w',  0,    0},
    {"clobber", 		x'C', x'C', OPT_ALL},
    {"completealiases", 	0,    0,    0},
    {"completeinword", 		0,    0,    0},
    {"correct", 		'0',  0,    0},
    {"correctall", 		'O',  0,    0},
    {"cshjunkiehistory", 	0,    0,    OPT_EMULATE|OPT_CSH},
    {"cshjunkieloops", 		0,    0,    OPT_EMULATE|OPT_CSH},
    {"cshjunkiequotes", 	0,    0,    OPT_EMULATE|OPT_CSH},
    {"cshnullglob", 		0,    0,    OPT_EMULATE|OPT_CSH},
    {"equals", 			0,    0,    OPT_EMULATE|OPT_ZSH},
    {"errexit", 		'e',  'e',  0},
    {"exec", 			x'n', x'n', OPT_ALL},
    {"extendedglob", 		0,    0,    0},
    {"extendedhistory", 	0,    0,    OPT_EMULATE|OPT_CSH},
    {"flowcontrol", 		0,    0,    OPT_ALL},
    {"functionargzero",		0,    0,    OPT_EMULATE|OPT_NONBOURNE},
    {"glob", 			x'F', x'f', OPT_ALL},
    {"globassign", 		0,    0,    OPT_EMULATE|OPT_CSH},
    {"globcomplete", 		0,    0,    0},
    {"globdots", 		'4',  0,    0},
    {"globsubst", 		0,    0,    OPT_EMULATE|OPT_NONZSH},
    {"hashcmds", 		0,    0,    OPT_ALL},
    {"hashdirs", 		0,    0,    OPT_ALL},
    {"hashlistall", 		0,    0,    OPT_ALL},
    {"histallowclobber", 	0,    0,    0},
    {"histbeep", 		0,    0,    OPT_ALL},
    {"histignoredups", 		'h',  0,    0},
    {"histignorespace", 	'g',  0,    0},
    {"histnostore", 		0,    0,    0},
    {"histverify", 		0,    0,    0},
    {"hup", 			0,    0,    OPT_EMULATE|OPT_ZSH},
    {"ignorebraces", 		'I',  0,    OPT_EMULATE|OPT_SH},
    {"ignoreeof", 		'7',  0,    0},
    {"interactive", 		'i',  'i',  OPT_SPECIAL},
    {"interactivecomments", 	'k',  0,    OPT_EMULATE|OPT_BOURNE},
    {"ksharrays", 		0,    0,    OPT_EMULATE|OPT_BOURNE},
    {"kshoptionprint", 		0,    0,    OPT_EMULATE|OPT_KSH},
    {"listambiguous", 		0,    0,    0},
    {"listbeep", 		0,    0,    OPT_ALL},
    {"listtypes", 		'X',  0,    OPT_CSH},
    {"localoptions", 		0,    0,    OPT_EMULATE|OPT_KSH},
    {"login", 			'l',  'l',  OPT_SPECIAL},
    {"longlistjobs", 		'R',  0,    0},
    {"magicequalsubst", 	0,    0,    0},
    {"mailwarning", 		'U',  0,    0},
    {"markdirs", 		'8',  'X',  0},
    {"menucomplete", 		'Y',  0,    0},
    {"monitor", 		'm',  'm',  OPT_SPECIAL},
    {"multios", 		0,    0,    OPT_EMULATE|OPT_ZSH},
    {"nomatch", 		x'3', 0,    OPT_EMULATE|OPT_NONBOURNE},
    {"notify", 			'5',  'b',  OPT_ZSH},
    {"nullglob", 		'G',  0,    OPT_EMULATE},
    {"numericglobsort", 	0,    0,    0},
    {"overstrike", 		0,    0,    0},
    {"pathdirs", 		'Q',  0,    0},
    {"posixbuiltins",		0,    0,    OPT_EMULATE|OPT_BOURNE},
    {"printeightbit", 		0,  0,    0},
    {"printexitvalue", 		'1',  0,    0},
    {"privileged", 		'p',  'p',  OPT_SPECIAL},
    {"promptcr", 		x'V', 0,    OPT_ALL},
    {"promptsubst", 		0,    0,    OPT_EMULATE|OPT_KSH},
    {"pushdignoredups", 	0,    0,    0},
    {"pushdminus", 		0,    0,    0},
    {"pushdsilent", 		'E',  0,    0},
    {"pushdtohome", 		'D',  0,    0},
    {"rcexpandparam", 		'P',  0,    0},
    {"rcquotes", 		0,    0,    0},
    {"rcs", 			x'f', 0,    OPT_ALL},
    {"recexact", 		'S',  0,    0},
    {"rmstarsilent", 		'H',  0,    OPT_BOURNE},
    {"shfileexpansion",		0,    0,    OPT_EMULATE|OPT_BOURNE},
    {"shglob", 			0,    0,    OPT_EMULATE|OPT_BOURNE},
    {"shinstdin", 		's',  's',  OPT_SPECIAL},
    {"shoptionletters",		0,    0,    OPT_EMULATE|OPT_BOURNE},
    {"shortloops", 		0,    0,    OPT_ALL},
    {"shwordsplit", 		'y',  0,    OPT_EMULATE|OPT_BOURNE},
    {"singlecommand",		't',  't',  OPT_SPECIAL},
    {"singlelinezle", 		'M',  0,    OPT_KSH},
    {"sunkeyboardhack", 	'L',  0,    0},
    {"unset", 			x'u', x'u', OPT_EMULATE|OPT_BSHELL},
    {"verbose", 		'v',  'v',  0},
#ifdef WINNT
    {"winntconvertbackslash", 	0,  0,  0},
    {"winntignorecase", 	0,  0,  0},
    {"winntlamepathfix",  	0,  0,  0},
    {"winntnoassociations",	0,  0,  0},
    {"winntnoquoteprotect",	0,  0,  0},
    {"winntwaitforguiapps",	0,  0,  0},
#endif WINNT
    {"xtrace", 			'x',  'x',  0},
    {"zle", 			'Z',  0,    OPT_SPECIAL},
};
# undef x
#endif

EXTERN short int typtab[256] INIT_ZERO_STRUCT;
#if defined (WINNT)
#ifndef GLOBALS
#undef INIT_ZERO
#undef INIT_ZERO_STRUCT
#define INIT_ZERO =0
#define INIT_ZERO_STRUCT ={0}
#endif GLOBALS
#endif WINNT
