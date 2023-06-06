#include "tree.cpp"

#include "dcel.hpp"
#include "util.hpp"

#include <gtest/gtest.h>

TEST(TreeTests, general) {
    std::vector<vec2> vertices = {
        vec2(0, 7),
        vec2(3, 4),
        vec2(2, 6),
        vec2(4, 5),
        vec2(5, 6),
        vec2(1, 0),
        vec2(6, 3),
        vec2(5, 1),
        vec2(8, 4),
        vec2(7, 6),
    };

    DCEL dcel(vertices);

    Tree t(dcel);
    
    HalfEdgeId a = dcel.start();
    HalfEdgeId b = dcel.next(dcel.next(dcel.start()));
    HalfEdgeId c = dcel.next(dcel.next(dcel.next(dcel.next(dcel.start()))));
    HalfEdgeId d = dcel.next(dcel.next(dcel.next(dcel.next(dcel.next(dcel.next(dcel.start()))))));

    
    t.insert(a);
    t.insert(b);
    t.insert(c);
    //t.insert(d);
    
    EXPECT_EQ(t.get(vec2(2.5, 5.5)), a);
    EXPECT_EQ(t.get(vec2(3.5, 5.5)), b);
    EXPECT_EQ(t.get(vec2(5, 5)), c);
    EXPECT_EQ(t.get(vec2(7, 2)), c);
}
