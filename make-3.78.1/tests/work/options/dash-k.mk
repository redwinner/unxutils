VPATH = work
edit:  main.o kbd.o commands.o display.o 
	@echo cc -o edit main.o kbd.o commands.o display.o 
main.o : main.c defs.h
	@echo cc -c main.c
kbd.o : kbd.c defs.h command.h
	@echo cc -c kbd.c
commands.o : command.c defs.h command.h
	@echo cc -c commands.c
display.o : display.c defs.h buffer.h
	@echo cc -c display.c
