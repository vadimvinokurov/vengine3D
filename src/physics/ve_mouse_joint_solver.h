//
// Created by boris on 12/23/2021.
//

#ifndef VENGINE3D_VE_MOUSE_JOINT_SOLVER_H
#define VENGINE3D_VE_MOUSE_JOINT_SOLVER_H


namespace VE {
    class MouseJointSolver {
    public:
        MouseJointSolver(RigidBodyPtr body, const Vector3 &jointPoint) : body(body) {
            bodyLocalPoint = body->globalToLocalPoint(jointPoint);
        }

        void applyImpulse(float dt, const Vector3 &mousePoint) {
            VE::Vector3 bp = body->localToGlobalPoint(bodyLocalPoint);

            Vector3 cPos = bp - mousePoint;

            Vector3 r = bp - body->centerOfMass();
            VE::Matrix3 E;

            VE::Matrix3 R(0.0f, r.z, -r.y,
                           -r.z, 0.0f, r.x,
                           r.y, -r.x, 0.0f);


            VE::Vector3 cVel = body->linearVelocity() + body->angularVelocity() * r;
            VE::Matrix3 effectiveMass = (E * body->invMass() + R * body->invInertia() * R.getTransposed()).getInversed();

            VE::Vector3 b = cPos * beta / dt;
            VE::Vector3 lymbda = effectiveMass * ((cVel + b) * -1);
            float maxImpulse = maxForce * dt;
            lymbda = lymbda.getNormalized() * std::min(lymbda.len(), maxImpulse);

            body->setLinearVelocity(body->linearVelocity() + lymbda * body->invMass());
            body->setAngularVelocity(body->angularVelocity() + body->invInertia() * (r * lymbda));

            bp.drawPoint(12);
        }

    private:
        RigidBodyPtr body;
        Vector3 bodyLocalPoint;

        float maxForce = 20000.0f;
        float beta = 0.2f;
    };

    using MouseJointSolverPtr = std::shared_ptr<VE::MouseJointSolver>;
}


#endif //VENGINE3D_VE_MOUSE_JOINT_SOLVER_H
