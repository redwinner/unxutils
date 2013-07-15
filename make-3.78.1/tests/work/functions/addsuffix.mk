string := $(addsuffix .c,src/a.b.z.foo hacks) 
all: 
	@echo $(string) 
