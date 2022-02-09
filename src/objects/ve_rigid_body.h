//
// Created by boris on 11/25/2021.
//

#ifndef VENGINE3D_VE_RIGID_BODY_H
#define VENGINE3D_VE_RIGID_BODY_H

#include "ve_collider.h"
#include "math/ve_transform.h"
#include "ve_stdlibs.h"

namespace VE {
    class RigidBody;

    using RigidBodyPtr = std::shared_ptr<VE::RigidBody>;

    class RigidBody {
    public:
        explicit RigidBody(std::vector<ColliderPtr> &&collidersPtr);
        static RigidBodyPtr create(std::vector<ColliderPtr> &&collidersPtr);
        void setTransform(const Transform &transform);

        void updateVelocity(float dt);
        void updateTransform(float dt);

        void setGravity(const Vector3 &gravity);
        void addForce(const Vector3 &force);
        void addForce(const Vector3 &force, const Vector3 &forcePoint);

        void setPosition(const Vector3 &position);
        void setLinearVelocity(const Vector3 &linearVelocity);
        void setAngularVelocity(const Vector3 &angularVelocity);
        const Vector3 &linearVelocity() const;
        const Vector3 &angularVelocity() const;

        void setPseudoLinearVelocity(const Vector3 &pseudoLinearVelocity);
        void setPseudoAngularVelocity(const Vector3 &pseudoAngularVelocity);
        const Vector3 &pseudoLinearVelocity() const;
        const Vector3 &pseudoAngularVelocity() const;

        Vector3 globalToLocalPoint(const Vector3 &globalPoint);
        Vector3 localToGlobalPoint(const Vector3 &localPoint);

        float invMass() const;
        float restitution() const;
        float friction() const;

        void setRestitution(float restitution);

        const Vector3 &centerOfMass() const;
        const Matrix3 &invInertia() const;


        const std::vector<ColliderPtr> &colliders() const;
        const Transform &transform() const;
        virtual ~RigidBody();


        void setColor(const Color &color) const { color_ = color; }

        Color color() const { return color_; }

    private:
        void computeMass();

        std::vector<ColliderPtr> colliders_;
        Transform transform_;
        Matrix3 invInertia_;
        Vector3 centerOfMass_;
        float invMass_;


        Vector3 force_;
        Vector3 torque_;
        Vector3 gravity_ = Vector3();
        Vector3 linearVelocity_;
        Vector3 angularVelocity_;
        Vector3 pseudoLinearVelocity_;
        Vector3 pseudoAngularVelocity_;

        float sleepEpsilont_ = 0.001f;
        float damping_ = 0.999f;
        float restitution_ = 0.0f;
        float friction_ = 0.5f;

        mutable Color color_ = Color(0.5f, 0.5f, 0.5f);
    };
}


#endif //VENGINE3D_VE_RIGID_BODY_H
