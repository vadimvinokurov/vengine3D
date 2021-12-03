//
// Created by boris on 12/3/2021.
//

#ifndef VENGINE3D_VE_EPA_FACE_H
#define VENGINE3D_VE_EPA_FACE_H

#include "Math/ve_vector.h"

namespace VE {
    namespace EPA {
        struct Face {
            Face(unsigned int i1, unsigned int i2, unsigned int i3) : index{i1, i2, i3} {}

            unsigned int index[3];
            Vector normal;
            float distance = 0.0f;
            bool actualData = false;
        };
    }
}

#endif //VENGINE3D_VE_EPA_FACE_H
