//
// Created by boris on 11/23/2021.
//

#ifndef VENGINE3D_VE_TRANSFORM_H
#define VENGINE3D_VE_TRANSFORM_H

#include "ve_vector.h"

namespace VE {
    class Transform {
    public:
        Transform() : scale(1, 1, 1),
                      rotation(0, 0, 0),
                      position(0, 0, 0) {
        }

        VE::Vector scale;
        VE::Vector rotation;
        VE::Vector position;
    };
}


#endif //VENGINE3D_VE_TRANSFORM_H
