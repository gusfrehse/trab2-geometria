#include "util.hpp"

#include <list>
#include <vector>

class DCEL {
    std::vector<DCELVertex> vertices;
    std::vector<DCELHalfEdge> halfEdges;
    std::vector<DCELFace> faces;
    
public:
    DCEL(std::initializer_list<float> init) {
        for (auto it = init.begin(); it != init.end(); it += 2) {
            vertices.push_back({ { *it, *(it + 1) } });
        }
    }
};

struct DCELVertex {
    vec2 coords;
    DCELHalfEdge *incidentEdge = nullptr;

};

struct DCELFace {
    DCELHalfEdge *outerComponent = nullptr;
    std::list<DCELHalfEdge *> innerComponents;
};

struct DCELHalfEdge {
    DCELVertex *origin = nullptr;
    DCELHalfEdge *twin = nullptr;
    DCELFace *incidentFace = nullptr;
    
    DCELHalfEdge *next = nullptr, *prev = nullptr;
};