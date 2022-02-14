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
        Track<Vector3> & getPositionTrack();
        Track<Quaternion>& getRotationTrack();
        Track<Vector3>& getScaleTrack();
        float getStartTime();
        float getEndTime();
        bool isValid();
        Transform sample(const Transform& ref, float time, bool looping);
    protected:
        unsigned int id_;
        Track<Vector3> position_;
        Track<Quaternion> rotation_;
        Track<Vector3> scale_;
    };
}


#endif //VENGINE3D_VE_TRANSFORM_TRACK_H
