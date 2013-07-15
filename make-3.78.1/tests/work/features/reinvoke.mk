
all: ; @echo 'running rules.'

work/features/reinvoke.mk work/features/reinvoke.mk.1: work/features/reinvoke.mk.2
	@echo 'rebuilding $@.'
	@echo >> $@

include work/features/reinvoke.mk.1

