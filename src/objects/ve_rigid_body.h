//
// Created by boris on 11/25/2021.
//

#ifndef VENGINE3D_VE_RIGID_BODY_H
#define VENGINE3D_VE_RIGID_BODY_H

#include "ve_collider.h"
#include "Math/ve_transform.h"
#include "stdlibraries.h"

namespace VE {
    class RigidBody {
    public:
        RigidBody();
        void addCollider(const ColliderPtr &constShapePtr);
        void setTransform(const Transform &transform);
        void moveTo(Vector dp);

        void updateVelocity(float dt);
        void updateTransform(float dt);

        void setGravity(const Vector &gravity);
        void addForce(const Vector &force);
        void addForce(const Vector &force, const Vector &forcePoint);

        void setLinearVelocity(const Vector &linearVelocity);
        void setAngularVelocity(const Vector &angularVelocity);
        const Vector &linearVelocity() const;
        const Vector &angularVelocity() const;

        void setPseudoLinearVelocity(const Vector &pseudoLinearVelocity);
        void setPseudoAngularVelocity(const Vector &pseudoAngularVelocity);
        const Vector &pseudoLinearVelocity() const;
        const Vector &pseudoAngularVelocity() const;

        Vector globalToLocalPoint(const Vector &globalPoint);

        float invMass() const;
        float restitution() const;

        const Vector &centerOfMass() const;
        const Matrix33 &invInertia() const;


        size_t collidersSize() const;
        const Collider &collider(size_t n) const;
        const Transform &transform() const;
        virtual ~RigidBody();
    private:
        void computeMass();

        std::vector<ColliderPtr> colliders_;
        Transform transform_;
        Matrix33 invInertia_;
        Vector centerOfMass_;
        float invMass_;


        Vector force_;
        Vector torque_;
        Vector gravity_ = Vector();
        Vector linearVelocity_;
        Vector angularVelocity_;
        Vector pseudoLinearVelocity_;
        Vector pseudoAngularVelocity_;

        float sleepEpsilont_ = 0.0001f;
        float damping_ = 0.999f;
        float restitution_ = 0.0f;
    };

    using RigidBodyPtr = std::shared_ptr<VE::RigidBody>;
}


#endif //VENGINE3D_VE_RIGID_BODY_H