//
// Created by boris on 12/13/2021.
//

#ifndef VENGINE3D_VE_CONTACT_SOLVER_H
#define VENGINE3D_VE_CONTACT_SOLVER_H

#include "objects/ve_rigid_body.h"
#include "collision/ve_collision_system.h"
#include "ve_global_parameters.h"

namespace VE {
    struct ContactKey {
        ContactKey(VE::RigidBody &body1, VE::RigidBody &body2) : bodyptr1(&body1), bodyptr2(&body2) {
            if (&body1 < &body2) {
                bodyptr1 = &body2;
                bodyptr2 = &body1;
            }
        }

        bool operator<(const VE::ContactKey &b) const {
            if (bodyptr1 < b.bodyptr1)
                return true;
            return bodyptr1 == b.bodyptr1 && bodyptr2 < b.bodyptr2;
        }

        VE::RigidBody *bodyptr1;
        VE::RigidBody *bodyptr2;
    };

    namespace ContactSolverParametrs {
        const float speedSlop = 0.15f;
        const float penetrationSlop = 0.01f;
        const float beta = 0.2f;
        const float normalDeviation = 0.99f;
        const float contactSlop = 0.01f;
    };

    class ContactSolver {
    public:
        ContactSolver(VE::RigidBody &body1, VE::RigidBody &body2);

        bool isCollide();

        void preStep(float dt);
        void applyImpulse(float dt);
        void applyPseudoImpulse(float dt);
        const ContactMainfold &contactMainfold();

        void update(ContactMainfold newContactMainfold);
    private:
        float computeEffectiveMass(const Vector3 &nCrossR1, const Vector3 &nCrossR2);

        ContactMainfold contactMainfold_;
        VE::RigidBody &body1;
        VE::RigidBody &body2;
        bool contact_;
        float friction_;
        float restitution_;
    };
}

#endif //VENGINE3D_VE_CONTACT_SOLVER_H
