#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <queue>
#include <set>

#include "util.hpp"
#include "make_monotone.hpp"
#include "dcel.hpp"

void readVertices(int n, std::vector<vec2>& vertices) {
    vertices.resize(n);

    for (int i = 0; i < n; ++i) {
        vertices[i] = vec2::read();
    }
}


int main() {
    int n;

    std::cin >> n;

    std::vector<vec2> vertices;

    readVertices(n, vertices);

    DCEL dcel = makeMonotone(vertices);

    dcel.print();

    return 0;
}
