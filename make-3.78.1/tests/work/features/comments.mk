# Test comment vs semicolon parsing and line continuation
target: # this ; is just a comment \
	@echo This is within a comment. 
	@echo There should be no errors for this makefile.
