//
// Created by boris on 2/12/2022.
//

#ifndef VENGINE3D_VE_DEBUG_DRAW_H
#define VENGINE3D_VE_DEBUG_DRAW_H

#include "math/ve_vector.h"

namespace VE {
    void debugDrawVector(const Vector3 &vector3, const Vector3 &basePoint = Vector3(), const VE::Color &color = VE::Color());
    void debugDrawPoint(const Vector3 &vector3, int size = 6, const VE::Color &color = VE::Color());
}

#endif //VENGINE3D_VE_DEBUG_DRAW_H
