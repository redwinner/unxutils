VPATH = work/

GPATH = $(VPATH)

.SUFFIXES: .a .b .c .d
.PHONY: general rename notarget intermediate

%.a:
%.b:
%.c:
%.d:

%.a : %.b ; cat $^ > $@
%.b : %.c ; cat $^ > $@
%.c :: %.d ; cat $^ > $@

# General testing info:

general: foo.b
foo.b: foo.c bar.c

