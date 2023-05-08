#include <iostream>

#include "util.hpp"

struct triangle {
    std::array<int, 3> vertices;
    std::array<int, 3> neighbours;
}

int main() {
    int n;
    std::vector<vec2<float>> polygon;

    std::cin >> n;

    polygon.resize(n);

    for (int i = 0; i < n; i++) {
        float x, y;

        std::cin >> x >> y;

        polygon[i] = vec2(x, y);
    }

    return 0;
}
