files = foo.elc bar.o lose.o   

$(filter %.o,$(files)): %.o: %.c
	@echo CC -c $(CFLAGS) $< -o $@ 
$(filter %.elc,$(files)): %.elc: %.el 
	@echo emacs $< 
