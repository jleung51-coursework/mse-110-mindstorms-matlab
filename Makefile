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
	@echo "INSTRUCTIONS:"
	@echo "  To test Exercise 1 (pong), run: make pong"
	@echo "  To test Project 1 (line follower), run: make follower"
	@echo "  To test Project 1 (line follower PID), run: make follower-pid"
	@echo "  To test Project 3 (barcode scanner), run: make scanner"
	@echo "  To remove compiled output files, run: make clean"

# Executed whenever an object file is out of date
# Symbols:
#   % refers to any character(s)
#   $< refers to the first item in the dependency list
%.o: %.c $(HEADERS)  #TODO Use the path to the .c files, while keeping %.c - e.g. ../src/%.c
	$(GCC) $(CFLAGS) $<

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

# $ make scanner
scanner: project-3-barcode-scanner/scanner.o
	$(GCC) scanner.o -o $(OUTPUT)
	@echo "To execute the program, run: ./$(OUTPUT)"

# $ make clean
# Removes created files
clean:
	rm -f $(OUTPUT) *.o *~
