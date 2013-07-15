all: one.x two.x three.x
FOO = foo
BAR = bar
BAZ = baz
thr% : override BAZ = three
t%.x: BAR = four
%.x: BAR = two
%.x: override BAZ = three
one.x: override FOO = one
one.x two.x three.x: ; @echo $(FOO) $(BAR) $(BAZ)
