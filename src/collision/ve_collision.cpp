//
// Created by boris on 12/13/2021.
//

#include "ve_collision.h"
#include "collision/gjk/ve_gjk.h"
#include "collision/epa/ve_epa.h"
#include "ve_contact_points.h"

bool VE::testIntersection(const VE::Collider &collider1, const VE::Collider &collider2) {
    return GJK(collider1, collider2).testIntersection();
}

bool VE::testIntersection(const VE::Collider &collider1, const VE::Collider &collider2, VE::Vector &contactPenetration) {
    auto gkj = GJK(collider1, collider2);
    if(gkj.testIntersection()){
        contactPenetration = EPA::PenetrationDepth(collider1, collider2, gkj.getSimplex()).getVector();
        return true;
    }
    return false;
}

bool VE::testIntersection(const VE::SphereCollider &sphere1, const VE::SphereCollider &sphere2, VE::Vector &contactPenetration) {
    Vector sphereDistanceVector = sphere2.getCenterOfMass() - sphere1.getCenterOfMass();
    float radiusSum = sphere1.getRadius() + sphere2.getRadius();
    float sphereDistance = sphereDistanceVector.abs();
    contactPenetration = sphereDistanceVector.normolize() * (radiusSum - sphereDistance);
    return sphereDistance < radiusSum;
}

bool VE::testIntersection(const VE::RigidBody &body1, const VE::RigidBody &body2) {
    for (size_t i = 0; i < body1.collidersSize(); i++) {
        for (size_t j = 0; j < body2.collidersSize(); j++) {
            if (GJK(body1.collider(i), body2.collider(j)).testIntersection()) {
                return true;
            }
        }
    }
    return false;
}

bool VE::testIntersection(const VE::RigidBody &body1, const VE::RigidBody &body2, VE::ContactMainfold &contactMainfold) {
    contactMainfold.clear();
    Vector contactPenetration;
    for (size_t i = 0; i < body1.collidersSize(); i++) {
        for (size_t j = 0; j < body2.collidersSize(); j++) {



            if (testIntersection(body1.collider(i), body2.collider(j),contactPenetration)) {
                Vector normal = contactPenetration.normolize();
                float collisionDepth = contactPenetration.abs();

//                auto contactPoints = VE::ContactPoint(static_cast<const VE::BoxCollider &>(body1.collider(i)),
//                                                      static_cast<const VE::BoxCollider &>(body2.collider(j)),
//                                                      normal).get();

//                for (auto& contactPoint: contactPoints) {
//                    contactMainfold.emplace_back(contactPoint, normal, collisionDepth);
//                }
            }

        }
    }
    return !contactMainfold.empty();
}

