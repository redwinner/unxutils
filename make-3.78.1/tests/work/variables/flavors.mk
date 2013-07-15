foo = $(bar)
bar = ${ugh}
ugh = Hello

all: multi ; @echo $(foo)

multi: ; $(multi)

x := foo
y := $(x) bar
x := later

nullstring :=
space := $(nullstring) $(nullstring)

next: ; @echo $x$(space)$y

define multi
@echo hi
@echo there
endef

ifdef BOGUS
define
@echo error
endef
endif

