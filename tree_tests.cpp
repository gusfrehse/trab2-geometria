#include "tree.cpp"

#include "dcel.hpp"
#include "util.hpp"

#include <gtest/gtest.h>

//TEST(TreeTests, general) {
//    std::vector<vec2> vertices = {
//        vec2(0, 7),
//        vec2(3, 4),
//        vec2(2, 6),
//        vec2(4, 5),
//        vec2(5, 6),
//        vec2(1, 0),
//        vec2(6, 3),
//        vec2(5, 1),
//        vec2(8, 4),
//        vec2(7, 6),
//    };
//
//    DCEL dcel(vertices);
//
//    Tree t(dcel);
//    
//    HalfEdgeId a = dcel.start();
//    HalfEdgeId b = dcel.next(dcel.next(dcel.start()));
//    HalfEdgeId c = dcel.next(dcel.next(dcel.next(dcel.next(dcel.start()))));
//    HalfEdgeId d = dcel.next(dcel.next(dcel.next(dcel.next(dcel.next(dcel.next(dcel.start()))))));
//
//    
//    t.insert(a);
//    t.insert(b);
//    t.insert(c);
//    //t.insert(d);
//    
//    EXPECT_EQ(t.get(vec2(2.5, 5.5)), a);
//    EXPECT_EQ(t.get(vec2(3.5, 5.5)), b);
//    EXPECT_EQ(t.get(vec2(5, 5)), c);
//    EXPECT_EQ(t.get(vec2(7, 2)), c);
//}

TEST(TreeTests, general) {
    std::vector<vec2> vertices;

    // make a triangle with dents in the bottom
    for (int i = 0; i <= 16; i++)
    {
        vertices.push_back(vec2(i, 2 * (i % 2)));
        std::cout << vertices[i] << ", ";
    }

    vertices.push_back(vec2(8, 100));

    std::cout << vertices[vertices.size() - 1] << std::endl;


    DCEL dcel(vertices);

    Tree t(dcel);
    
    // TODO!
    HalfEdgeId e34 = dcel.prev(dcel.start());
    HalfEdgeId e2 = dcel.next(dcel.next(e34));
    HalfEdgeId e6 = dcel.next(dcel.next(e2));
    HalfEdgeId e10 = dcel.next(dcel.next(e6));
    HalfEdgeId e14 = dcel.next(dcel.next(e10));
    HalfEdgeId e18 = dcel.next(dcel.next(e14));
    HalfEdgeId e22 = dcel.next(dcel.next(e18));
    HalfEdgeId e26 = dcel.next(dcel.next(e22));
    HalfEdgeId e30 = dcel.next(dcel.next(e26));

    std::cout << "e34 " << e34 << " " << dcel.getHalfEdge(e34) << std::endl;
    std::cout << "e2  "  << e2 << " " << dcel.getHalfEdge(e2) << std::endl;
    std::cout << "e6  "  << e6 << " " << dcel.getHalfEdge(e6) << std::endl;
    std::cout << "e10  "  << e10 << " " << dcel.getHalfEdge(e10) << std::endl;
    std::cout << "e14  "  << e14 << " " << dcel.getHalfEdge(e14) << std::endl;
    std::cout << "e18  "  << e18 << " " << dcel.getHalfEdge(e18) << std::endl;
    std::cout << "e22  "  << e22 << " " << dcel.getHalfEdge(e22) << std::endl;
    std::cout << "e26  "  << e26 << " " << dcel.getHalfEdge(e26) << std::endl;
    std::cout << "e30 " << e30 << " " << dcel.getHalfEdge(e30) << std::endl;

    std::cout << e34 << " " << dcel.getHalfEdge(e34) << std::endl;

    t.insert(e14);
    t.insert(e30);
    t.insert(e18);
    t.insert(e6);
    t.insert(e34);
    t.insert(e26);
    t.insert(e10);
    t.insert(e22);
    t.insert(e2);

    // insert only downward edges (to the left of the polygon)
    t.print();
    
    HalfEdgeId a = dcel.prev(dcel.start());
    for (int i = 0; i < 9; i++)
    {
        EXPECT_EQ(t.get(vec2(2 * i + 1, 1)), a);
        a = dcel.next(dcel.next(a));
    }

    t.remove(e26);
    
    t.print();
}

