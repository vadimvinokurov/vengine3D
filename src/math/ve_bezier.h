//
// Created by boris on 2/12/2022.
//

#ifndef VENGINE3D_VE_BEZIER_H
#define VENGINE3D_VE_BEZIER_H

namespace VE {
    template<typename T>
    class Bezier {
    public:
        Bezier(const T &P1_, const T &C1_, const T &P2_, const T &C2_) :
                P1(P1_), C1(C1_), P2(P2_), C2(C2_) {}

        T interpolate(float t) {
//            auto A = T::lerp(P1, C1, t);
//            auto C = T::lerp(C1, C2, t);
//            auto B = T::lerp(C2, P2, t);
//            auto E = T::lerp(A, C, t);
//            auto F = T::lerp(C, B, t);
//            auto R = T::lerp(E, F, t);
//            return R;

            return P1 * ((1 - t) * (1 - t) * (1 - t)) +
                   C1 * (3.0f * ((1 - t) * (1 - t)) * t) +
                   C2 * (3.0f * (1 - t) * (t * t)) +
                   P2 * (t * t * t);
        }


        T P1;
        T C1;
        T P2;
        T C2;
    };
}

#endif //VENGINE3D_VE_BEZIER_H
