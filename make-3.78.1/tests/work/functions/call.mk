# Simple, just reverse something
#
reverse = $2 $1

# A complex `map' function, using recursive `call'.
#
map = $(foreach a,$2,$(call $1,$a))

# Test using a builtin; this is silly as it's simpler to do without call
#
my-notdir = $(call notdir,$(1))

all: ; @echo '$(call reverse,bar,foo)'; \
        echo '$(call map,origin,MAKE reverse map)'; \
        echo '$(call my-notdir,a/b   c/d      e/f)'

