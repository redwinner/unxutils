NEQ = $(subst $1,,$2)

all:
	@echo $(if    ,true,false)
	@echo $(if ,true,)
	@echo $(if ,true)
	@echo $(if z,true,false)
	@echo $(if z,true,$(shell echo hi))
	@echo $(if ,$(shell echo hi),false)
	@echo $(if $(call NEQ,a,b),true,false)
	@echo $(if $(call NEQ,a,a),true,false)
	@echo $(if z,true,fal,se)
	@echo $(if ,true,fal,se)
