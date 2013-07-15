ifdef ERROR1
$(error error is $(ERROR1))
endif

ifdef ERROR2
$(error error is $(ERROR2))
endif

ifdef ERROR3
all: some; @echo $(error error is $(ERROR3))
endif

ifdef ERROR4
all: some; @echo error is $(ERROR4)
	@echo $(error error is $(ERROR4))
endif

some: ; @echo Some stuff

