//
// Created by boris on 11/25/2021.
//

#ifndef VENGINE3D_VE_RIGID_BODY_HPP
#define VENGINE3D_VE_RIGID_BODY_HPP

#include "ve_shape.h"
#include "Math/ve_transform.h"

namespace VE{
    class RigidBody {
    public:
        RigidBody();

        virtual ~RigidBody();
    private:
        VE::Transform transform;
    };
}



#endif //VENGINE3D_VE_RIGID_BODY_HPP
