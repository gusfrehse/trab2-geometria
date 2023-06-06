
#include "make_monotone.cpp"

TEST(MakeMonotoneHelpers, ToTheLeft) {
    std::vector<vec2> vertices = { vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1) };

    dcel.insertVertices(vertices);
    
    EXPECT_TRUE(toTheLeft(dcel.prev(dcel.start()), dcel.next(dcel.start())));
}