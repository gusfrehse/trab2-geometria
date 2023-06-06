CC = g++
CXX = g++
CXXFLAGS += -Wall -Wextra -Wpedantic  -pedantic-errors -O2 -g -std=c++20
OBJS = dcel.o make_monotone.o
TEST_OBJS = dcel_tests.o
MAIN = triangulate.o

triangulate : $(MAIN) $(OBJS)

test : LDLIBS += -lgtest -lgtest_main -lpthread
test : $(OBJS) $(TEST_OBJS)
	g++ -o $@ $(LDFLAGS) $(OBJS) $(TEST_OBJS) $(LOADLIBES) $(LDLIBS)

triangulate.o : triangulate.cpp make_monotone.hpp util.hpp
make_monotone.o : make_monotone.cpp dcel.hpp util.hpp
dcel.o : dcel.cpp dcel.hpp util.hpp
dcel_tests.o: dcel_tests.cpp dcel.hpp util.hpp


run : triangulate
	./triangulate < input.txt

clean :
	rm -f $(TEST_OBJS) $(OBJS) $(MAIN) triangulate test
