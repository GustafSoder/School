# Compiler
CC=gcc

# Compiler flags
CFLAGS= -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition

# Force remove
RM=rm -f

# Main program linking
mdu: mdu.o queue.o
		$(CC) -pthread mdu.o queue.o -o mdu

# Separate file compilation
mdu.o: mdu.c
		$(CC) $(CFLAGS) -c mdu.c -o mdu.o

queue.o: queue.c queue.h
		$(CC) $(CFLAGS) -c queue.c -o queue.o

# Clean workspace from built files
clean:
		$(RM) mdu *.o
