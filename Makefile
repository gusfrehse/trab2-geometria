CC = g++
CXX = g++
CXXFLAGS += -Wall -Wextra -Wpedantic  -pedantic-errors -O0 -g -std=c++20
OBJS = dcel.o make_monotone.o triangulate_monotone.o tree.o
TEST_OBJS = dcel_tests.o make_monotone_tests.o tree_tests.o
MAIN = triangulate.o

triangulate : $(MAIN) $(OBJS)

test : LDLIBS += -lgtest -lgtest_main -lpthread
test : $(OBJS) $(TEST_OBJS)
	g++ -o $@ $(CXXFLAGS) $(LDFLAGS)  $(TEST_OBJS) $(LOADLIBES) $(LDLIBS)

triangulate.o : triangulate.cpp make_monotone.hpp triangulate_monotone.hpp util.hpp
triangulate_monotone.o : triangulate_monotone.cpp triangulate_monotone.hpp dcel.hpp util.hpp
make_monotone.o : make_monotone.cpp dcel.hpp util.hpp tree.hpp
make_monotone_tests.o: make_monotone_tests.cpp dcel.hpp util.hpp
dcel.o : dcel.cpp dcel.hpp util.hpp
dcel_tests.o: dcel_tests.cpp dcel.hpp util.hpp
tree.o : tree.cpp tree.hpp dcel.hpp util.hpp
tree_tests.o: tree_tests.cpp tree.hpp dcel.hpp util.hpp


run : triangulate
	./triangulate < input.txt

clean :
	rm -f $(TEST_OBJS) $(OBJS) $(MAIN) triangulate test
