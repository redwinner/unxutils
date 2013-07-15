string  := word.pl general_test2.pl1 FORCE.pl word.pl3 generic_test.perl /tmp.c/bar foo.baz/bar.c MAKEFILES_variable.c
string2 := $(string) $(string) $(string) $(string) $(string) $(string) $(string)
string3 := $(string2) $(string2) $(string2) $(string2) $(string2) $(string2) $(string2)
string4 := $(string3) $(string3) $(string3) $(string3) $(string3) $(string3) $(string3)
all: 
	@echo $(suffix $(string)) 
	@echo $(sort $(suffix $(string4))) 
	@echo $(suffix $(string) a.out) 
	@echo $(sort $(suffix $(string3))) 
