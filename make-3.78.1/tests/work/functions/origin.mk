foo := bletch garf
auto_var = udef CC HOME MAKE foo CFLAGS WHITE @
av = $(foreach var, $(auto_var), $(origin $(var)) )
override WHITE := BLACK
all: auto
	@echo $(origin undefined)
	@echo $(origin CC)
	@echo $(origin HOME)
	@echo $(origin MAKE)
	@echo $(origin foo)
	@echo $(origin CFLAGS)
	@echo $(origin WHITE)
	@echo $(origin @)
auto :
	@echo $(av)
