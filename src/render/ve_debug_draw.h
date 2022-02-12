//
// Created by boris on 2/12/2022.
//

#ifndef VENGINE3D_VE_DEBUG_DRAW_H
#define VENGINE3D_VE_DEBUG_DRAW_H

#include "math/ve_vector.h"

namespace VE {
    struct DebugDraw{
        static void Vector(const Vector3 &vector3, const Vector3 &basePoint = Vector3(), const VE::Color &color = VE::Color());
        static void Point(const Vector3 &vector3, int size = 6, const VE::Color &color = VE::Color());
        static void Line(const Vector3 &A, const Vector3 &B, const VE::Color &color = VE::Color());

        static unsigned int colorShaderSlot;
    };
}

#endif //VENGINE3D_VE_DEBUG_DRAW_H
