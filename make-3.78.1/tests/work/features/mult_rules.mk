objects = foo.o bar.o
foo.o : defs.h
bar.o : defs.h test.h
extradeps = 
$(objects) : config.h $(extradeps) 
	@echo EXTRA EXTRA
