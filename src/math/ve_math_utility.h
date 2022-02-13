//
// Created by boris on 2/13/2022.
//

#ifndef VENGINE3D_VE_MATH_UTILITY_H
#define VENGINE3D_VE_MATH_UTILITY_H

#include "ve_splines.h"

namespace VE{
    enum class Interpolation{
        Constant,
        Linear,
        Cubic
    };

    inline float linearInterpolate(float a, float b, float t) {
        return a + (b - a) * t;
    }

    inline Vector3 linearInterpolate(const Vector3 &a, const Vector3 &b, float t) {
        return Vector3::lerp(a, b, t);
    }

    inline Quaternion linearInterpolate(const Quaternion &a, const Quaternion &b, float t) {
        if (a.dot(b) < 0.0f) {
            return Quaternion::mix(a, b * -1.0f, t).normalize();
        } else {
            return Quaternion::mix(a, b, t).normalize();
        }
    }

    inline float loopclamp(float val, float lo, float hi) {
        val = fmodf(val - lo, hi - lo);
        return val < 0.0f ? val + hi : val + lo;
    };

}

#endif //VENGINE3D_VE_MATH_UTILITY_H
