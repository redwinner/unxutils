foo = bletch null @ garf
null :=
space = ' '
auto_var = udef space CC null PATH MAKE foo CFLAGS WHITE @ <
av = $(foreach var, $(auto_var), $(origin $(var)) )
override WHITE := BLACK
for_var = $(addsuffix .c,foo $(null) $(foo) $(space) $(av) )
fe = $(foreach var2, $(for_var),$(subst .c,.o, $(var2) ) )
all: auto for2
auto :
	@echo $(av)
for2:
	@echo $(fe)
