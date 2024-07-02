# Compiler
CC=gcc

# Compiler flags
CFLAGS= -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition

# Force remove
RM=rm -f

# Main program linking
mexec: mexec.o
		$(CC) mexec.o -o mexec

# Separate file compilation
mexec.o: mexec.c
		$(CC) $(CFLAGS) -c mexec.c -o mexec.o

# Clean workspace from built files
clean:
		$(RM) mexec *.o
