//
// Created by boris on 11/28/2021.
//

#ifndef VENGINE3D_VE_GJK_H
#define VENGINE3D_VE_GJK_H

#include "objects/ve_collider.h"

namespace VE {
    class GJK {
    public:
        GJK(const Collider &collider1, const Collider &collider2);
        bool testIntersection();
        const std::vector<VE::Vector3>& getSimplex() const;
    private:

        bool nextSimplex();
        bool lineCase();
        bool triangleCase();
        bool tetrahedronCase();

        const Collider &collider1_;
        const Collider &collider2_;
        std::vector<VE::Vector3> simplex;
        VE::Vector3 direction = Vector3(1, 1, 1);

        bool originIntoTriangleFace = false;
    };
}


#endif //VENGINE3D_VE_GJK_H
