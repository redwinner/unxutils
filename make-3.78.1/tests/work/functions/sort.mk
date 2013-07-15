foo := moon_light days 
foo1:= jazz
bar := captured 
bar2 = boy end, has rise A midnight 
bar3:= $(foo)
s1  := _by
s2  := _and_a
t1  := $(addsuffix $(s1), $(bar) )
t2  := $(addsuffix $(s2), $(foo1) )
t3  := $(t2) $(t2) $(t2) $(t2) $(t2) $(t2) $(t2) $(t2) $(t2) $(t2) 
t4  := $(t3) $(t3) $(t3) $(t3) $(t3) $(t3) $(t3) $(t3) $(t3) $(t3) 
t5  := $(t4) $(t4) $(t4) $(t4) $(t4) $(t4) $(t4) $(t4) $(t4) $(t4) 
t6  := $(t5) $(t5) $(t5) $(t5) $(t5) $(t5) $(t5) $(t5) $(t5) $(t5) 
t7  := $(t6) $(t6) $(t6) 
p1  := $(addprefix $(foo1), $(s2) )
blank:= 
all:
	@echo $(sort $(bar2) $(foo)  $(addsuffix $(s1), $(bar) ) $(t2) $(bar2) $(bar3))
	@echo $(sort $(blank) $(foo) $(bar2) $(t1) $(p1) )
	@echo $(sort $(foo) $(bar2) $(t1) $(t4) $(t5) $(t7) $(t6) )
