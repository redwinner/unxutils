#Extra space at the end of the following file name
include work/features/include.mk.1                  
all: ; @echo There should be no errors for this makefile.

-include nonexistent.mk
-include nonexistent.mk
sinclude nonexistent.mk
sinclude nonexistent-2.mk
-include makeit.mk
sinclude makeit.mk

error: makeit.mk
