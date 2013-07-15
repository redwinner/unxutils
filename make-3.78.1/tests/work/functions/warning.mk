ifdef WARNING1
$(warning warning is $(WARNING1))
endif

ifdef WARNING2
$(warning warning is $(WARNING2))
endif

ifdef WARNING3
all: some; @echo hi $(warning warning is $(WARNING3))
endif

ifdef WARNING4
all: some; @echo hi
	@echo there $(warning warning is $(WARNING4))
endif

some: ; @echo Some stuff

