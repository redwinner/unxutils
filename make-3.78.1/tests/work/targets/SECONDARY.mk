
.SECONDARY: foo.e

# Implicit rule test
%.d : %.e ; cp $< $@
%.e : %.f ; cp $< $@

foo.d: foo.e

# Explicit rule test
foo.c: foo.e ; cp $< $@
