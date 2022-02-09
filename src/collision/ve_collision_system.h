//
// Created by boris on 1/10/2022.
//

#ifndef VENGINE3D_COLLISIONSYSTEM_H
#define VENGINE3D_COLLISIONSYSTEM_H

#include "ve_contact_manifold.h"
#include "objects/ve_colliders.h"
#include "collision/ve_intersection.h"
#include "ve_contact_points.h"

namespace VE {
    inline bool getContactMainfold(const VE::RigidBody &body1, const VE::RigidBody &body2, VE::ContactMainfold &contactMainfold) {
        contactMainfold.clear();
        Vector3 contactPenetration;
        for (size_t i = 0; i < body1.collidersSize(); i++) {
            for (size_t j = 0; j < body2.collidersSize(); j++) {


                if (testIntersection(body1.collider(i), body2.collider(j), contactPenetration)) {
                    auto[normal, collisionDepth] = contactPenetration.getNormalAndLen();

                    auto contactPoints = VE::getContactsPoint(body1.collider(i), body2.collider(j), normal);

                    for (auto &contactPoint: contactPoints) {
                        contactMainfold.emplace_back(contactPoint, normal, collisionDepth);
                    }
                }

            }
        }
        return !contactMainfold.empty();
    }
}


#endif //VENGINE3D_COLLISIONSYSTEM_H
