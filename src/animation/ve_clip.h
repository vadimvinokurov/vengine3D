//
// Created by boris on 2/14/2022.
//

#ifndef VENGINE3D_VE_CLIP_H
#define VENGINE3D_VE_CLIP_H

#include <stdlibraries.h>
#include "ve_transform_track.h"
#include "ve_pose.h"

namespace VE {
    class Clip {
    public:
        Clip();
        float sample(Pose &outPose, float time);
        void recalculateDuration();
        TransformTrack &operator[](unsigned int jointId);

        unsigned int size();
        unsigned int getIdAtIndex(unsigned int index);
        void setIdAtIndex(unsigned int index, unsigned int jointId);
        std::string &getName();
        void setName(const std::string name);
        float getDuration();
        float getStartTime();
        float getEndTime();
        bool getLooping();
        void setLooping(bool looping);
    private:
        float agjustTimeToFitRange(float time);

        std::vector<TransformTrack> tracks_;
        std::string name_;
        float startTime_;
        float endTime_;
        bool looping_;
    };
}


#endif //VENGINE3D_VE_CLIP_H
