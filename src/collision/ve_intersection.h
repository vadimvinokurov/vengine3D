//
// Created by boris on 1/10/2022.
//

#ifndef VENGINE3D_VE_INTERSECTION_H
#define VENGINE3D_VE_INTERSECTION_H

#include "objects/ve_colliders.h"
#include "collision/gjk/ve_gjk.h"
#include "collision/epa/ve_epa.h"

namespace VE {
    inline bool testColliderIntersection(const VE::Collider &collider1, const VE::Collider &collider2, VE::Vector &contactPenetration) {
        auto gkj = GJK(collider1, collider2);
        if (gkj.testIntersection()) {
            contactPenetration = EPA::PenetrationDepth(collider1, collider2, gkj.getSimplex()).getVector();
            return true;
        }
        return false;
    }

    inline bool testSpheresIntersection(const SphereCollider &sphere1, const SphereCollider &sphere2, Vector &contactPenetration) {
        Vector sphereDistanceVector = sphere2.getCenterOfMass() - sphere1.getCenterOfMass();
        float radiusSum = sphere1.getRadius() + sphere2.getRadius();
        float sphereDistance = sphereDistanceVector.len();
        contactPenetration = sphereDistanceVector.getNormalized() * (radiusSum - sphereDistance);
        return sphereDistance < radiusSum;
    }

    inline bool testIntersection(const VE::Collider &collider1, const VE::Collider &collider2, VE::Vector &contactPenetration) {
        if(collider1.shapeType() == ColliderType::sphere && collider2.shapeType() == ColliderType::sphere){
            return testSpheresIntersection(static_cast<const SphereCollider &>(collider1), static_cast<const SphereCollider &>(collider2), contactPenetration);
        } else {
            return testColliderIntersection(collider1, collider2, contactPenetration);
        }
    }
}


#endif //VENGINE3D_VE_INTERSECTION_H
