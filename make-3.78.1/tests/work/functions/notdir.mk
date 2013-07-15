string := $(notdir /src/foo.c hacks) 
all: 
	@echo $(string) 
