
#include <gtest/gtest.h>

#include "util.hpp"
#include "dcel.cpp"

void printCycle(DCEL &dcel, HalfEdgeId start) {
    HalfEdgeId curr = start;

    do {
        std::cout << "HalfEdge: " << curr << " " << dcel.getHalfEdge(curr) << std::endl;
        curr = dcel.next(curr);
    } while (curr != dcel.start());
}

TEST(DCELTests, EmptyVectorConstructor) {
    std::vector<vec2> empty;
    DCEL dcel(empty);
    EXPECT_EQ(dcel.start(), 0);
}

TEST(DCELTests, QuadTest) {
    std::vector<vec2> vertices = { vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1) };

    DCEL dcel(vertices);

    EXPECT_EQ(dcel.getVertex(dcel.start()).coords, vec2(0, 0));

    // test next
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.next(dcel.incidentEdge(dcel.start())))).coords, vec2(1.0f, 0.0f));
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.next(dcel.next(dcel.incidentEdge(dcel.start()))))).coords, vec2(1.0f, 1.0f));
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.next(dcel.next(dcel.next(dcel.incidentEdge(dcel.start())))))).coords, vec2(0.0f, 1.0f));
    EXPECT_EQ(dcel.origin(dcel.next(dcel.next(dcel.next(dcel.next(dcel.incidentEdge(dcel.start())))))), dcel.start());

    // test prev
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.prev(dcel.start()))).coords, vec2(0.0f, 1.0f));
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.prev(dcel.prev(dcel.start())))).coords, vec2(1.0f, 1.0f));
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.prev(dcel.prev(dcel.prev(dcel.start()))))).coords, vec2(1.0f, 0.0f));
    EXPECT_EQ(dcel.prev(dcel.prev(dcel.prev(dcel.prev(dcel.start())))), dcel.start());
}

TEST(DCELTests, ConnectTest) {
    std::vector<vec2> vertices = { vec2(0, 0), vec2(1, 0), vec2(1, 0), vec2(0, 1) };

    DCEL dcel(vertices);
    
    dcel.connect(dcel.next(dcel.incidentEdge(dcel.start())), dcel.next(dcel.next(dcel.next(dcel.incidentEdge(dcel.start())))));

    // first vertex
    EXPECT_EQ(dcel.getVertex(dcel.start()).coords.x, 0.0f) << "First vertex has wrong x coordinate";
    EXPECT_EQ(dcel.getVertex(dcel.start()).coords.y, 0.0f) << "First vertex has wrong y coordinate";

    // second vertex
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.next(dcel.incidentEdge(dcel.start())))).coords.x, 1.0f) << "Second vertex has wrong x coordinate";
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.next(dcel.incidentEdge(dcel.start())))).coords.y, 0.0f) << "Second vertex has wrong y coordinate";

    // fourth (!) vertex
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.next(dcel.next(dcel.incidentEdge(dcel.start()))))).coords.x, 0.0f) << "Fourth vertex has wrong x coordinate";
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.next(dcel.next(dcel.incidentEdge(dcel.start()))))).coords.y, 1.0f) << "Fourth vertex has wrong y coordinate";
}