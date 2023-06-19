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
    VertexId v0 = -1;
    VertexId v1 = -1;
    VertexId v2 = -1;
    FaceId f0 = -1;
    FaceId f1 = -1;
    FaceId f2 = -1;
};

// O(n^2) n is the size of (sub) polygon
HalfEdgeId findEar(DCEL& dcel, HalfEdgeId start) {
    HalfEdgeId current = start;
    do {
        if (dcel.ear(current))
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
    std::vector<Triangle> triangles;

    readVertices(n, vertices);

    DCEL dcel(vertices);

    dcel.setupEars();

    HalfEdgeId v0, v1, v2, v3;
    HalfEdgeId head = dcel.start();

    int left = dcel.vertices.size();

    while (left > 3)
    {
        HalfEdgeId current = head;

        do
        {
            if (dcel.ear(current))
            {
                v2 = current;
                v1 = dcel.prev(v2);
                v0 = dcel.prev(v1);
                v3 = dcel.next(v2);
                
                FaceId face = dcel.connect(v1, v3);

                triangles.push_back((Triangle) {
                    .v0 = dcel.origin(v1),
                    .v1 = dcel.origin(v2),
                    .v2 = dcel.origin(v3),
                    .f0 = dcel.incidentFace(dcel.twin(v2)),
                    .f1 = face,
                    .f2 = dcel.incidentFace(dcel.twin(v1))
                });

                dcel.ear(dcel.next(v0)) = dcel.isEar(dcel.next(v0));
                dcel.ear(v3) = dcel.isEar(v3);

                head = v3;
                left--;
                break;
            }

            current = dcel.next(current);
        }
        while (current != head);
    }

    std::cout << vertices.size() << std::endl;
    for (auto &v : vertices) {
        std::cout << v << std::endl;
    }

    std::cout << triangles.size() << std::endl;
    for (auto &triangle : triangles)
    {
        std::cout << triangle.v0 + 1 << " " << triangle.v1 + 1 << " " << triangle.v2 + 1 << " " << triangle.f0 + 1 << " " << triangle.f1 + 1 << " " << triangle.f2 + 1 << std::endl;
    }

    //dcel.print();
    
    return 0;
}
