CC = g++
CXX = g++
CXXFLAGS += -Wall -Wextra -Wpedantic -pedantic-errors -O2 -g
OBJS = triangulate.o

triangulate : $(OBJS)

triangulate.o : triangulate.cpp util.hpp

run : triangulate
	./triangulate < input.txt
