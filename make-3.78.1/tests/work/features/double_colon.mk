foo:: bar.h 
	@echo Executing rule foo FIRST
foo2: bar.h 
foo:: bar2.h 
	@echo Executing rule foo SECOND
