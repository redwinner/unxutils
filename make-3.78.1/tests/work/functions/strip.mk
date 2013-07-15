TEST1 := "Is this TERMINAL fun?                                                               What makes you believe is this terminal fun?                                                                                                                                               JAPAN is a WONDERFUL planet -- I wonder if we will ever reach                                         their level of COMPARATIVE SHOPPING..."
E :=
TEST2 := $E   try this     and		this     	$E

define TEST3

and these	        test out


some
blank lines



endef

.PHONY: all
all:
	@echo '$(strip  $(TEST1) )'
	@echo '$(strip  $(TEST2) )'
	@echo '$(strip  $(TEST3) )'

