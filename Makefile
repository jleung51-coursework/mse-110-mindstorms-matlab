# Header files
HEADERS = \
	robotc_dummy.h

# gcc options
GCC = gcc -std=c11 -Wall -Wextra -Wmissing-declarations -Werror -Wno-unknown-pragmas -Wno-unused-parameter

# Compiler options
CFLAGS = -c

# Output filename
OUTPUT = program.out

#####
###
#
# 'Make' commands
#
###
#####

# $ 'make' or 'make help'
help:
	@echo "ROBOTC MAKEFILE INSTRUCTIONS:"
	@echo "  To enable the ROBOTC dummy code to allow GCC compilation through this Makefile, run: make activate_dummy"
	@echo "  To disable the ROBOTC dummy file to disallow GCC compilation through this Makefile, run: make deactivate_dummy"
	@echo "  To compile all ROBOTC files, run: make all"
	@echo "  To compile Exercise 1 (pong), run: make pong"
	@echo "  To compile Project 1 (line follower), run: make follower"
	@echo "  To compile Project 1 (line follower PID), run: make follower-pid"
	@echo "  To compile Project 2 (combat), run: make combat"
	@echo "  To compile Project 3 (barcode scanner - ROBOTC only), run: make scanner"
	@echo "  To compile Project 4 (maze navigator), run: make maze"
	@echo "  To remove compiled output files, run: make clean"

# Executed whenever an object file is out of date
# Symbols:
#   % refers to any character(s)
#   $< refers to the first item in the dependency list
%.o: %.c $(HEADERS)
	$(GCC) $(CFLAGS) $<

activate_dummy:
	./scripts/activate_robotc_dummy_code.sh

deactivate_dummy:
	./scripts/deactivate_robotc_dummy_code.sh

# $ make all
all: pong follower follower-pid combat scanner

# $ make pong
pong: exercise-1-pong/pong.o
	$(GCC) pong.o -o $(OUTPUT)
	@echo "To execute the program, run: ./$(OUTPUT)"

# $ make follower
follower: project-1-line-follower/line-follower.o
	$(GCC) line-follower.o -o $(OUTPUT)
	@echo "To execute the program, run: ./$(OUTPUT)"

# $ make follower-pid
follower-pid: project-1-line-follower/line-follower-pid.o
	$(GCC) line-follower-pid.o -o $(OUTPUT)
	@echo "To execute the program, run: ./$(OUTPUT)"

# $ make combat
combat: project-2-combat/combat.o
	$(GCC) combat.o -o $(OUTPUT)
	@echo "To execute the program, run: ./$(OUTPUT)"

# $ make scanner
scanner: project-3-barcode-scanner/scanner.o
	$(GCC) scanner.o -o $(OUTPUT)
	@echo "To execute the program, run: ./$(OUTPUT)"

# $ make maze
maze: project-4-maze-navigator/maze-navigator.o project-4-maze-navigator/stack.h project-4-maze-navigator/map-essentials.h
	$(GCC) maze-navigator.o -o $(OUTPUT)
	@echo "To execute the program, run: ./$(OUTPUT)"

# $ make clean
# Removes created files
clean:
	rm -f $(OUTPUT) *.o *~
