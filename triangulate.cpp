#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <queue>
#include <set>

#include "util.hpp"
#include "dcel.hpp"

void readVertices(int n, std::vector<vec2>& vertices) {
    vertices.resize(n);

    for (int i = 0; i < n; ++i) {
        vertices[i] = vec2::read();
    }
}

struct Triangle
{
    
};

HalfEdgeId findEar(DCEL& dcel, HalfEdgeId start) {
    HalfEdgeId current = start;
    do {
        if (dcel.isEar(current))
            return current;
        current = dcel.next(current);
    }
    while (current != start);

    return -1;
}

int main() {
    int n;

    std::cin >> n;

    std::vector<vec2> vertices;

    readVertices(n, vertices);

    DCEL dcel(vertices);
    
    HalfEdgeId current = dcel.start();
    
    HalfEdgeId ear;
    
   while ((ear = findEar(dcel, current)) != -1)
    {
        HalfEdgeId next = dcel.next(ear);
        dcel.connect(next, dcel.prev(ear));
        current = next;
    }
   // 
   // for (int i = 0; i < dcel.faces.size(); i++) {

   // }

    dcel.print();
    
    return 0;
}
