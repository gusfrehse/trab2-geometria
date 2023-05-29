
#include <gtest/gtest.h>

#include "util.hpp"
#include "dcel.hpp"

TEST(DCELTests, EmptyVectorConstructor) {
    std::vector<vec2> empty;
    DCEL dcel(empty);
    EXPECT_EQ(dcel.start(), nullptr);
    EXPECT_EQ(dcel.end(), nullptr);
}

TEST(DCELTests, TriangleTest) {
    std::vector<vec2> vertices = {vec2(0, 0), vec2(1, 0), vec2(0, 1)};

    DCEL dcel(vertices);

    EXPECT_EQ(dcel.start()->coords, vec2(0, 0));
    EXPECT_EQ(dcel.start()->incidentEdge->twin->origin->coords.x, 1.0f);
    EXPECT_EQ(dcel.start()->incidentEdge->twin->origin->coords.y, 0.0f);
    EXPECT_EQ(dcel.end()->coords, vec2(0.0f, 1.0f));
    EXPECT_EQ(dcel.start()->incidentEdge->incidentFace, dcel.end()->incidentEdge->incidentFace);
}

TEST(DCELTests, ConnectTest) {
    std::vector<vec2> vertices = {vec2(0, 0), vec2(1, 0), vec2(1, 0), vec2(0, 1)};

    DCEL dcel(vertices);


    dcel.connect(dcel.start(), dcel.start()->incidentEdge->next->next->origin);

    DCELVertex *start = dcel.start();
    DCELVertex *curr = dcel.start();

    do {
        std::cout << "Vertex: ";
        curr->coords.print();
        curr = curr->incidentEdge->twin->next->origin;
    } while (curr != dcel.start());

    // first vertex
    EXPECT_EQ(dcel.start()->coords.x, 0.0f);
    EXPECT_EQ(dcel.start()->coords.y, 0.0f);

    // second vertex
    EXPECT_EQ(dcel.start()->incidentEdge->next->origin->coords.x, 1.0f);
    EXPECT_EQ(dcel.start()->incidentEdge->next->origin->coords.y, 0.0f);

    // fourth (!) vertex
    EXPECT_EQ(dcel.start()->incidentEdge->next->next->origin->coords.x, 0.0f);
    EXPECT_EQ(dcel.start()->incidentEdge->next->next->origin->coords.y, 1.0f);
}