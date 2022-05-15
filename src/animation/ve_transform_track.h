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
        TransformTrack() = default;
		TransformTrack(const TransformTrack& tack) = default;
		TransformTrack(TransformTrack&& tack) = default;
		TransformTrack& operator=(const TransformTrack& tack) = default;
		TransformTrack& operator=(TransformTrack&& tack) = default;
		~TransformTrack() = default;

        void setJointIndex(std::size_t jointIndex);
        std::size_t getJointIndex() const;
        float getStartTime() const;
        float getEndTime() const;
        bool isValid() const;
        void sample(Transform &transform, float time, bool looping) const;

        Track<Vector3> position;
        Track<Quaternion> rotation;
        Track<Vector3> scale;
    protected:
        std::size_t jointIndex_ = 0;

    };
}


#endif //VENGINE3D_VE_TRANSFORM_TRACK_H
