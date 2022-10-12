# Makefile for CSF Assignment 3

GCC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: csim.o functions.o
	$(GCC) -o csim csim.o functions.o

csim.o: csim.c functions.h
	$(GCC) -c csim.c $(CFLAGS)

functions.o: functions.c functions.h
	$(GCC) $(CFLAGS) -c functions.c

clean:
	rm -f *.o hw3 test *~
