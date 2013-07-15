print1: ;@echo $(wildcard example.*)
print2:
	@echo $(wildcard example.?)
	@echo $(wildcard example.[a-z0-9])
	@echo $(wildcard example.[!A-Za-z_\!])
clean:
	rm $(wildcard example.*)
