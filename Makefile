CC = g++
CXX = g++
CXXFLAGS += -Wall -Wextra -O0 -g -std=c++20
OBJS = triangulate.o dcel.o 

triangulate : $(OBJS)

triangulate.o : triangulate.cpp  util.hpp
dcel.o : dcel.cpp dcel.hpp util.hpp

run : triangulate
	./triangulate < input.txt

clean :
	rm -f $(TEST_OBJS) $(OBJS) $(MAIN) triangulate test
