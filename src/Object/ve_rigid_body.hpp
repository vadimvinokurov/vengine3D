//
// Created by boris on 11/25/2021.
//

#ifndef VENGINE3D_VE_RIGID_BODY_HPP
#define VENGINE3D_VE_RIGID_BODY_HPP

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

        size_t collidersSize();
        const Collider &collider(size_t n);
        const Transform &transform() const;
        virtual ~RigidBody();
    private:
        std::vector<ColliderPtr> colliders_;
        Transform transform_;


    };

    using RigidBodyPtr = std::shared_ptr<VE::RigidBody>;
}


#endif //VENGINE3D_VE_RIGID_BODY_HPP
