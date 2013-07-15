SHELL = /bin/sh
export FOO = foo
export BAR = bar
one: override FOO = one
one two: ; @echo $(FOO) $(BAR)
two: BAR = two
three: ; BAR=1000
	@echo $(FOO) $(BAR)
# Some things that shouldn't be target vars
funk : override
funk : override adelic
adelic override : ; echo $@
# Test per-target recursive variables
four:FOO=x
four:VAR$(FOO)=ok
four: ; @echo '$(FOO) $(VAR$(FOO)) $(VAR) $(VARx)'
five:FOO=x
five six : VAR$(FOO)=good
five six: ;@echo '$(FOO) $(VAR$(FOO)) $(VAR) $(VARx) $(VARfoo)'
# Test per-target variable inheritance
seven: eight
seven eight: ; @echo $@: $(FOO) $(BAR)
seven: BAR = seven
seven: FOO = seven
eight: BAR = eight
# Test the export keyword with per-target variables
nine: ; @echo $(FOO) $(BAR) $$FOO $$BAR
nine: FOO = wallace
# Test = escaping
EQ = =
ten: one\=two
ten: one \= two
ten one$(EQ)two $(EQ):;@echo $@
.PHONY: one two three four five six seven eight nine ten $(EQ) one$(EQ)two
# Test target-specific vars with pattern/suffix rules
QVAR = qvar
RVAR = =
%.q : ; @echo $(QVAR) $(RVAR)
foo.q : RVAR += rvar
# Target-specific vars with multiple LHS pattern rules
%.r %.s %.t: ; @echo $(QVAR) $(RVAR) $(SVAR) $(TVAR)
foo.r : RVAR += rvar
foo.t : TVAR := $(QVAR)
