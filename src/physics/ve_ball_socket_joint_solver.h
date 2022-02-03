//
// Created by boris on 1/11/2022.
//

#ifndef VENGINE3D_VE_BALL_SOCKET_JOINT_SOLVER_H
#define VENGINE3D_VE_BALL_SOCKET_JOINT_SOLVER_H

#include "objects/ve_rigid_body.h"

namespace VE {
    class BallAndSocketJointSolver {
    public:
        BallAndSocketJointSolver(RigidBodyPtr body1, RigidBodyPtr body2, const Vector &anchorPoint) :
                body1(body1),
                body2(body2) {
            body1anchor = body1->globalToLocalPoint(anchorPoint);
            body2anchor = body2->globalToLocalPoint(anchorPoint);
        }

        void applyImpulse(float dt) {
            VE::Vector r1 = body1->localToGlobalPoint(body1anchor) - body1->centerOfMass();
            VE::Vector r2 = body2->localToGlobalPoint(body2anchor) - body2->centerOfMass();

            VE::Vector cPos = body1->localToGlobalPoint(body1anchor) - body2->localToGlobalPoint(body2anchor);
            VE::Vector cVel = body1->linearVelocity() + body1->angularVelocity() * r1 - body2->linearVelocity() - body2->angularVelocity() * r2;

            VE::Matrix33 Jv1;
            VE::Matrix33 Jv2;
            Jv1.setIdentity();
            Jv2.setIdentity();
            Jv2 *= -1;


            VE::Matrix33 Jw1(0.0f, r1.z(), -r1.y(),
                             -r1.z(), 0.0f, r1.x(),
                             r1.y(), -r1.x(), 0.0f);
            VE::Matrix33 Jw2(0.0f, -r2.z(), r2.y(),
                             r2.z(), 0.0f, -r2.x(),
                             -r2.y(), r2.x(), 0.0f);

            VE::Matrix33 k = Jv1 * body1->invMass() * Jv1.getTranspose() + Jw1 * body1->invInertia() * Jw1.getTranspose() +
                             Jv2 * body2->invMass() * Jv2.getTranspose() + Jw2 * body2->invInertia() * Jw2.getTranspose();
            VE::Matrix33 effectiveMass = k.getInverse();

            VE::Vector b = cPos * beta / dt;
            VE::Vector lymbda = effectiveMass * ((cVel + b) * -1);

            body1->setLinearVelocity(body1->linearVelocity() + lymbda * body1->invMass());
            body1->setAngularVelocity(body1->angularVelocity() + (r1 * lymbda) * body1->invInertia());

            body2->setLinearVelocity(body2->linearVelocity() - lymbda * body2->invMass());
            body2->setAngularVelocity(body2->angularVelocity() - (r2 * lymbda) * body2->invInertia());
        }

    private:
        RigidBodyPtr body1;
        RigidBodyPtr body2;
        VE::Vector body1anchor;
        VE::Vector body2anchor;

        const float beta = 0.2f;
    };
}


#endif //VENGINE3D_VE_BALL_SOCKET_JOINT_SOLVER_H
