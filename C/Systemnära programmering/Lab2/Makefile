# Compiler
CC=gcc

# Compiler flags
CFLAGS= -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition

# Force remove
RM=rm -f

# Main program linking
mmake: mmake.o parser.o
		$(CC) mmake.o parser.o -o mmake 

# Separate file compilation
mmake.o: mmake.c parser.h
		$(CC) $(CFLAGS) -c mmake.c -o mmake.o 

parser.o: parser.c parser.h
		$(CC) $(CFLAGS) -c parser.c -o parser.o


# Clean workspace from built files
clean:
		$(RM) mmake mmake.o parser.o