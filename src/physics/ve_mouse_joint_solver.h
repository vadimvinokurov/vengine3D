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

        void setDebug(bool d) {
            debug = d;
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


            VE::Vector cVel = body->linearVelocity() + body->angularVelocity() * r;
            VE::Matrix33 effectiveMass = (E * body->invMass() + R * body->invInertia() * R.getTranspose()).getInverse();

            VE::Vector b = cPos * beta / dt;
            VE::Vector lymbda = effectiveMass * ((cVel + b) * -1);
            float maxImpulse = maxForce * dt;
            lymbda = lymbda.normolize() * std::min(lymbda.abs(), maxImpulse);

            body->setLinearVelocity(body->linearVelocity() + lymbda * body->invMass());
            body->setAngularVelocity(body->angularVelocity() + body->invInertia() * (r * lymbda));

            bp.drawPoint(12);
            r.draw(body->centerOfMass());
        }

    private:
        RigidBodyPtr body;
        Vector bodyLocalPoint;

        float maxForce = 20.0f;
        float beta = 0.2;
        bool debug = false;
    };

    using MouseJointSolverPtr = std::shared_ptr<VE::MouseJointSolver>;
}


#endif //VENGINE3D_VE_MOUSE_JOINT_SOLVER_H
