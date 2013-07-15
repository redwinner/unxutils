time_t getlogtime _((STRUCT_UTMP *u, int inout));
char * watch3ary _((int inout, STRUCT_UTMP *u, char *fmt, int prnt));
char * watchlog2 _((int inout, STRUCT_UTMP *u, char *fmt, int prnt, int fini));
void watchlog _((int inout, STRUCT_UTMP *u, char **w, char *fmt));
int ucmp _((STRUCT_UTMP *u, STRUCT_UTMP *v));
void readwtab _((void));
void dowatch _((void));
int bin_log _((char *nam, char **argv, char *ops, int func));
