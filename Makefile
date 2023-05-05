CC = g++
CXX = g++
CXXFLAGS += -Wall -Wextra -Wpedantic -pedantic-errors -O2 -g
OBJS = poligono.o

poligono : $(OBJS)

poligono.o : poligono.cpp util.hpp

