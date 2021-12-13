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
        void update(float dt);

        size_t collidersSize();
        const Collider &collider(size_t n);
        const Transform &transform() const;
        virtual ~RigidBody();
    private:
        void computeMass();

        std::vector<ColliderPtr> colliders_;
        Transform transform_;

        Vector centerOfMass_;
        float invMass_;
        Matrix33 invInertia_;


        Vector force_;
        Vector torque_;
        Vector linearVelocity_;
        Vector angularVelocity_;
    };

    using RigidBodyPtr = std::shared_ptr<VE::RigidBody>;
}


#endif //VENGINE3D_VE_RIGID_BODY_H
