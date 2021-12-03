//
// Created by boris on 11/28/2021.
//

#ifndef VENGINE3D_VE_SUPPORT_POINT_H
#define VENGINE3D_VE_SUPPORT_POINT_H

#include "ve_collision_common.h"
#include "objects/ve_collider.h"

namespace VE{
    struct SupportPoint {
        Vector point;
        Vector collider1Vertex;
        Vector collider2Vertex;
    };

    inline SupportPoint getSupportPoint(const Collider &collider1, const Collider &collider2, const VE::Vector &direction) {
        SupportPoint supportPoint;

        supportPoint.collider1Vertex = collider1.farthestVertexInDirection(direction);
        supportPoint.collider2Vertex = collider2.farthestVertexInDirection(direction * -1);

        supportPoint.point = supportPoint.collider1Vertex - supportPoint.collider2Vertex;
        return supportPoint;
    }
}


#endif //VENGINE3D_VE_SUPPORT_POINT_H
