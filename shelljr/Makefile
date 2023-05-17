# provide a makefile with a target called shell_jr that 
# creates the executable shell_jr

CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors -fstack-protector-all -Wextra

shell_jr: shell_jr.o
	$(CC) -o shell_jr shell_jr.o

shell_jr.o: shell_jr.c
	$(CC) $(CFLAGS) -c shell_jr.c

.PHONY: all clean

all: shell_jr

clean: 
	rm -rf *.o shell_jr