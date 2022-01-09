//
// Created by boris on 12/13/2021.
//

#ifndef VENGINE3D_VE_COLLISION_H
#define VENGINE3D_VE_COLLISION_H



#include <stdlibraries.h>
#include "objects/colliders.h"
#include "objects/ve_rigid_body.h"
#include "ve_contact_manifold.h"

namespace VE {
    bool testIntersection(const Collider &collider1, const Collider &collider2);
    bool testIntersection(const Collider &collider1, const Collider &collider2, Vector& contactPenetration);

    bool testIntersection(const SphereCollider &sphere1, const SphereCollider &sphere2, Vector& contactPenetration);

    bool testIntersection(const VE::RigidBody &body1, const VE::RigidBody &body2);

    bool testIntersection(const VE::RigidBody &body1, const VE::RigidBody &body2, VE::ContactMainfold &contactMainfold);
}

#endif //VENGINE3D_VE_COLLISION_H
