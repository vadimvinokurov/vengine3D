//
// Created by boris on 2/13/2022.
//

#ifndef VENGINE3D_VE_TRANSFORM_TRACK_H
#define VENGINE3D_VE_TRANSFORM_TRACK_H

#include "ve_track.h"
#include "math/ve_transform.h"

namespace VE {
    class TransformTrack {
    public:
        TransformTrack();
        void setId(unsigned int id);
        unsigned int getId();
        VectorTrack & getPositionTrack();
        QuaternionTrack& getRotationTrack();
        VectorTrack& getScaleTrack();
        float getStartTime();
        float getEndTime();
        bool isValid();
        Transform sample(const Transform& ref, float time, bool looping);
    protected:
        unsigned int id_;
        VectorTrack position_;
        QuaternionTrack rotation_;
        VectorTrack scale_;
    };
}


#endif //VENGINE3D_VE_TRANSFORM_TRACK_H
