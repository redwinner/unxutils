override define foo
@echo First comes the definition.
@echo Then comes the override.
endef
all: 
	$(foo)
