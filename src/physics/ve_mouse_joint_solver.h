//
// Created by boris on 12/23/2021.
//

#ifndef VENGINE3D_VE_MOUSE_JOINT_SOLVER_H
#define VENGINE3D_VE_MOUSE_JOINT_SOLVER_H


namespace VE {
    class MouseJointSolver {
    public:
        MouseJointSolver(RigidBodyPtr body, const Vector &jointPoint) : body(body) {
            bodyLocalPoint = body->globalToLocalPoint(jointPoint);
        }

        void applyImpulse(float dt, const Vector &mousePoint) {
            VE::Vector bp = body->localToGlobalPoint(bodyLocalPoint);
            Vector cPos = bp - mousePoint;

            Vector r = bp - body->centerOfMass();

            VE::Matrix33 E;
            E.setIdentity();

            VE::Matrix33 R(0.0f, r.z(), -r.y(),
                           -r.z(), 0.0f, r.x(),
                           r.y(), -r.x(), 0.0f);

            VE::Matrix33 k = E * body->invMass() * E.getTranspose() + R * body->invInertia() * R.getTranspose();
            VE::Matrix33 effectiveMass = k.getInverse();

            VE::Vector cVel = body->linearVelocity() + body->angularVelocity() * r;
            VE::Vector b = cPos * beta / dt;
            VE::Vector lymbda = effectiveMass * ((cVel + b) * -1);
            float maxImpulse = maxForce * dt;
            lymbda = lymbda.normolize() * std::min(lymbda.abs(), maxImpulse);


            body->setLinearVelocity(body->linearVelocity() + lymbda * body->invMass());
            body->setAngularVelocity(body->angularVelocity() + (r * lymbda) * body->invInertia());

        }

    private:
        RigidBodyPtr body;
        Vector bodyLocalPoint;

        const float maxForce = 5000.0f;
        const float beta = 0.2;
    };

    using MouseJointSolverPtr = std::shared_ptr<VE::MouseJointSolver>;
}


#endif //VENGINE3D_VE_MOUSE_JOINT_SOLVER_H
