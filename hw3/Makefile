# Makefile for CSF Assignment 3

CXX = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: csim.o helper_functions.o
	$(CXX) -o csim csim.o helper_functions.o

csim.o: csim.cpp 
	$(CXX) -c csim.cpp $(CFLAGS)

helper_functions.o: helper_functions.cpp helper_functions.h
	$(CXX) $(CFLAGS) -c helper_functions.cpp 

clean:
	rm -f *.o csim
