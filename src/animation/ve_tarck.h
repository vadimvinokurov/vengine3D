//
// Created by boris on 2/13/2022.
//

#ifndef VENGINE3D_VE_TARCK_H
#define VENGINE3D_VE_TARCK_H

#include "stdlibraries.h"
#include "ve_frame.h"
#include "ve_interpolation.h"
#include "math/ve_splines.h"
#include "math/ve_vector.h"
#include "math/ve_quaternion.h"

namespace VE {

    namespace TrackHelpers {
        inline float interpolate(float a, float b, float t) {
            return a + (b - a) * t;
        }

        inline Vector3 interpolate(const Vector3 &a, const Vector3 &b, float t) {
            return Vector3::lerp(a, b, t);
        }

        inline Quaternion interpolate(const Quaternion &a, const Quaternion &b, float t) {
            if (a.dot(b) < 0.0f) {
                auto res = Quaternion::mix(a, b * -1.0f, t);
                return res.normalize();
            } else {
                auto res = Quaternion::mix(a, b, t);
                return res.normalize();
            }
        }

        inline float adjustHermiteResult(float f) {
            return f;
        }

        inline Vector3 adjustHermiteResult(const Vector3 &v) {
            return v;
        }

        inline Quaternion adjustHermiteResult(const Quaternion &q) {
            return q.getNormalized();
        }

        inline void neighborhood(const float &a, float &b) {}

        inline void neighborhood(const Vector3 &a, Vector3 &b) {}

        inline void neighborhood(const Quaternion &a, Quaternion &b) {
            if (a.dot(b) < 0) {
                b *= -1.0f;
            }
        }
    }

    template<typename T, unsigned int N>
    class Track {
    public:
        Track();
        float getStartTime();
        float getEndTime();
        T sample(float time, bool looping);
        Frame<N> &operator[](unsigned int index);
        void resize(unsigned int size);
        unsigned int size();
        Interpolation getInterpolation();
        void setInterpolation(Interpolation interpolation);


    protected:
        static float loopclamp(float val, float lo, float hi) {
            float difference = hi - lo;
            val = fmodf(val - lo, difference);
            return val < 0.0f ? val + hi : val + lo;
        };

        T hermite(float t, const T &p1, const T &s1, const T &p2, const T &s2);
        int frameIndex(float time, bool looping);
        float adjustTimeToFitTrack(float time, bool looping);

        T sampleConstant(float time, bool looping);
        T sampleLinear(float time, bool looping);
        T sampleCubic(float time, bool looping);


        T cast(const float *value);

        std::vector<Frame<N>> frames_;
        Interpolation interpolation_;
    };

    using ScalarTrack = Track<float, 1>;
    using VectorTrack = Track<Vector3, 3>;
    using QuaternionTrack = Track<Quaternion, 4>;
}


#endif //VENGINE3D_VE_TARCK_H
