files := $(filter-out %.o, foo.elc bar.o lose.o) 
all: 
	@echo $(files) 
