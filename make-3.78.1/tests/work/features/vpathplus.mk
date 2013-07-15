VPATH = work/

SHELL = /bin/sh

.SUFFIXES: .a .b .c .d
.PHONY: general rename notarget intermediate

%.a:
%.b:
%.c:
%.d:

%.a : %.b
	cat $^ > $@
%.b : %.c
	cat $^ > $@ 2>/dev/null || exit 1
%.c :: %.d
	cat $^ > $@

# General testing info:

general: foo.b
foo.b: foo.c bar.c

# Rename testing info:

rename: $(VPATH)/foo.c foo.d

# Target not made testing info:

notarget: notarget.b
notarget.c: notarget.d
	-@echo "not creating $@ from $^"

# Intermediate files:

intermediate: inter.a

