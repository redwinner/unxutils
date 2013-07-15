int ingetc _((void));
int inputline _((void));
void inputsetline _((char *str, int flags));
void inungetc _((int c));
int stuff _((char *fn));
void inerrflush _((void));
void inpush _((char *str, int flags, Alias inalias));
void inpoptop _((void));
void inpop _((void));
