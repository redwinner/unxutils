SHELL = /bin/sh

define test
if [ ! -f test-file ]; then \
  echo >> test-file; sleep 2; rm -f test-file; \
else \
  echo $@ FAILED; \
fi
endef

all : ONE TWO THREE
ONE : ; @$(test)
TWO : ; @$(test)
THREE : ; @$(test)
