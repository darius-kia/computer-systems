CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors -fstack-protector-all -Wextra
PROGS = public01 public02 public03 public04 public05 student_tests

public01: public01.o calendar.o my_memory_checker_216.o
	$(CC) -o public01 public01.o calendar.o my_memory_checker_216.o

public01.o: public01.c calendar.h event.h
	$(CC) $(CLAGS) -c public01.c

public02: public02.o calendar.o my_memory_checker_216.o
	$(CC) -o public02 public02.o calendar.o my_memory_checker_216.o

public02.o: public02.c calendar.h event.h
	$(CC) $(CLAGS) -c public02.c

public03: public03.o calendar.o my_memory_checker_216.o
	$(CC) -o public03 public03.o calendar.o my_memory_checker_216.o

public03.o: public03.c calendar.h event.h
	$(CC) $(CLAGS) -c public03.c

public04: public04.o calendar.o my_memory_checker_216.o
	$(CC) -o public04 public04.o calendar.o my_memory_checker_216.o

public04.o: public04.c calendar.h event.h
	$(CC) $(CLAGS) -c public04.c

public05: public05.o calendar.o my_memory_checker_216.o
	$(CC) -o public05 public05.o calendar.o my_memory_checker_216.o

public05.o: public05.c calendar.h event.h
	$(CC) $(CLAGS) -c public05.c

student_tests: student_tests.o calendar.o my_memory_checker_216.o
	$(CC) -o student_tests student_tests.o calendar.o my_memory_checker_216.o

student_tests.o: student_tests.c
	$(CC) $(CFLAGS) -c student_tests.c

calendar.o: calendar.c calendar.h
	$(CC) $(CFLAGS) -c calendar.c

my_memory_checker_216.o: my_memory_checker_216.c my_memory_checker_216.h
	$(CC) $(CFLAGS) -c my_memory_checker_216.c

.PHONY: all clean

all: $(PROGS)

clean:
	rm -f *.o $(PROGS)
