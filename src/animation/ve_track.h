//
// Created by boris on 2/13/2022.
//

#ifndef VENGINE3D_VE_TRACK_H
#define VENGINE3D_VE_TRACK_H

#include "stdlibraries.h"
#include "ve_frame.h"
#include "math/ve_vector.h"
#include "math/ve_quaternion.h"
#include "math/ve_math_utility.h"
namespace VE {
    template<typename T>
    class Track {
    public:
        Track();
        float getStartTime();
        float getEndTime();
        T sample(float time, bool looping);
        Frame<T> &operator[](unsigned int index);
        void resize(unsigned int size);
        unsigned int size();
        Interpolation getInterpolation();
        void setInterpolation(Interpolation interpolation);
    private:
        T hermite(float t, const T &p1, const T &s1, const T &p2, const T &s2);
        int frameIndex(float time, bool looping);
        float adjustTimeToFitTrack(float time, bool looping);

        T sampleConstant(float time, bool looping);
        T sampleLinear(float time, bool looping);
        T sampleCubic(float time, bool looping);

        T cast(const float *value);
        T toT(const float *value);

        std::vector<Frame<T>> frames_;
        Interpolation interpolation_;
    };
}


#endif //VENGINE3D_VE_TRACK_H
