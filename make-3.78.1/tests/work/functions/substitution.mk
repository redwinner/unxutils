foo := a.o b.o c.o
bar := $(foo:.o=.c)
bar2:= $(foo:%.o=%.c)
bar3:= $(patsubst %.c,%.o,x.c.c bar.c)
all:
	@echo $(bar)
	@echo $(bar2)
	@echo $(bar3)
