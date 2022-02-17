//
// Created by boris on 2/13/2022.
//

#ifndef VENGINE3D_VE_TRANSFORM_TRACK_H
#define VENGINE3D_VE_TRANSFORM_TRACK_H

#include "ve_core.h"

#include "ve_track.h"
#include "math/ve_transform.h"

namespace VE {
    class TransformTrack {
    public:
        TransformTrack();
        void setJointId(unsigned int jointId);
        unsigned int getJointId() const;
        float getStartTime() const;
        float getEndTime() const;
        bool isValid() const;
        Transform sample(const Transform &ref, float time, bool looping) const;

        Track<Vector3> position;
        Track<Quaternion> rotation;
        Track<Vector3> scale;
    protected:
        unsigned int jointId_;

    };
}


#endif //VENGINE3D_VE_TRANSFORM_TRACK_H
