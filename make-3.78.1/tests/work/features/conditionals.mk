objects = foo.obj
arg1 = first
arg2 = second
arg3 = third
arg4 = cc
arg5 = second

all:
ifeq ($(arg1),$(arg2))
	@echo arg1 equals arg2
else
	@echo arg1 NOT equal arg2
endif

ifeq '$(arg2)' "$(arg5)"
	@echo arg2 equals arg5
else
	@echo arg2 NOT equal arg5
endif

ifneq '$(arg3)' '$(arg4)'
	@echo arg3 NOT equal arg4
else
	@echo arg3 equal arg4
endif

ifndef undefined
	@echo variable is undefined
else
	@echo variable undefined is defined
endif
ifdef arg4
	@echo arg4 is defined
else
	@echo arg4 is NOT defined
endif

