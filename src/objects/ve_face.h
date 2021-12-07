//
// Created by boris on 12/7/2021.
//

#ifndef VENGINE3D_VE_FACE_H
#define VENGINE3D_VE_FACE_H

#include <vector>
#include "math/ve_vector.h"

namespace VE {
    struct ColliderFace {
        ColliderFace(const std::vector<VE::Vector> &v, const VE::Vector &n) : vertices(v), normal(n) {}

        void projectionOn(const ColliderFace &refFace) {
            assert(refFace.normal.dot(normal) != 0.0f);
            for (auto &vertex: vertices) {
                float d = (refFace.vertices[0] - vertex).dot(refFace.normal);
                vertex = refFace.normal * d + vertex;
            }
        }

        std::vector<VE::Vector> vertices;
        VE::Vector normal;
    };
}


#endif //VENGINE3D_VE_FACE_H
