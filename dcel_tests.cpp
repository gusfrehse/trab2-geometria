
#include <gtest/gtest.h>

#include "util.hpp"
#include "dcel.hpp"

TEST(DCELTests, EmptyVectorConstructor) {
    std::vector<vec2> empty;
    DCEL dcel(empty);
    EXPECT_EQ(dcel.start(), 0);
}

TEST(DCELTests, TriangleTest) {
    std::vector<vec2> vertices = {vec2(0, 0), vec2(1, 0), vec2(0, 1)};

    DCEL dcel(vertices);

    EXPECT_EQ(dcel.getVertex(dcel.start()).coords, vec2(0, 0));
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.twin(dcel.incidentEdge(dcel.start())))).coords.x, 1.0f);
    EXPECT_EQ(dcel.getVertex(dcel.origin(dcel.twin(dcel.incidentEdge(dcel.start())))).coords.y, 0.0f);
    EXPECT_EQ(dcel.getVertex(dcel.end()).coords, vec2(0.0f, 1.0f));
}

TEST(DCELTests, ConnectTest) {
    std::vector<vec2> vertices = {vec2(0, 0), vec2(1, 0), vec2(1, 0), vec2(0, 1)};

    DCEL dcel(vertices);


    dcel.connect(dcel.start(), dcel.origin(dcel.next(dcel.next(dcel.incidentEdge(dcel.start())))));

    VertexId start = dcel.start();
    VertexId curr = dcel.start();

    do {
        std::cout << "Vertex: ";
        dcel.getVertex(curr).coords.print();
        curr = dcel.origin(dcel.next(dcel.twin(dcel.incidentEdge(curr))));
    } while (curr != dcel.start());

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