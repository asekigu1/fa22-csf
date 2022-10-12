# Makefile for CSF Assignment 3

CXX = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: csim.o functions.o
	$(CXX) -o csim csim.o functions.o

csim.o: csim.cpp functions.h
	$(CXX) -c csim.cpp $(CFLAGS)

functions.o: functions.c functions.h
	$(CXX) $(CFLAGS) -c functions.c

clean:
	rm -f *.o hw3 test *~
