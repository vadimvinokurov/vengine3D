//
// Created by boris on 2/12/2022.
//

#ifndef VENGINE3D_VE_SPLINES_H
#define VENGINE3D_VE_SPLINES_H

namespace VE {
    struct Spline {
        template<typename T>
        __forceinline static T Bezier(float t, const T &P1, const T &C1, const T &P2, const T &C2) noexcept {
            return P1 * ((1 - t) * (1 - t) * (1 - t)) +
                   C1 * (3.0f * ((1 - t) * (1 - t)) * t) +
                   C2 * (3.0f * (1 - t) * (t * t)) +
                   P2 * (t * t * t);
        }
        template<typename T>
        __forceinline static T Hermite(float t, const T &P1, const T &S1, const T &P2, const T &S2) noexcept {
            return P1 * ((1.0f + 2.0f * t) * ((1.0f - t) * (1.0f - t))) +
                   S1 * (t * ((1.0f - t) * (1.0f - t))) +
                   P2 * ((t * t) * (3.0f - 2.0f * t)) +
                   S2 * ((t * t) * (t - 1.0f));
        }
    };
}

#endif //VENGINE3D_VE_SPLINES_H
