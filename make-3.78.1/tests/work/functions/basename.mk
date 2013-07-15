string := $(basename src/a.b.z.foo.c src/hacks src.bar/a.b.z.foo.c src.bar/hacks hacks) 
all: 
	@echo $(string) 
