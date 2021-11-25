//
// Created by boris on 11/25/2021.
//

#ifndef VENGINE3D_VE_RIGID_BODY_HPP
#define VENGINE3D_VE_RIGID_BODY_HPP

#include "ve_shape.h"
#include "Math/ve_transform.h"
#include "stdlibraries.h"

namespace VE{
    class RigidBody {
    public:
        RigidBody();
        void addCollider(const VE::ShapePtr& shapePtr);
        void moveTo(VE::Vector dp);


        std::vector<VE::ShapePtr> colliders_;
        VE::Transform transform_;

        virtual ~RigidBody();
    };

    using RigidBodyPtr = std::shared_ptr<VE::RigidBody>;
}



#endif //VENGINE3D_VE_RIGID_BODY_HPP
