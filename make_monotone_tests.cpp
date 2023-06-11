
#include "make_monotone.cpp"



/*
TEST(MakeMonotoneHelpers, ToTheLeft) {
    std::vector<vec2> vertices = { vec2(0, 0), vec2(1, 1), vec2(2, 0), vec2(2, 2), vec2(0, 2) };

    dcel.insertVertices(vertices);
    
    HalfEdgeId curr = dcel.start();

    do {
        std::cout << "HalfEdge: " << curr << " " << dcel.getHalfEdge(curr) << std::endl;
        curr = dcel.next(curr);
    } while (curr != dcel.start());

    EXPECT_TRUE(toTheLeft(dcel.prev(dcel.start()), dcel.next(dcel.start())));
    EXPECT_FALSE(toTheLeft(dcel.next(dcel.start()), dcel.prev(dcel.start())));
}
*/