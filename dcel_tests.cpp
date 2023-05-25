
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