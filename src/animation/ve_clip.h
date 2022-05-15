//
// Created by boris on 2/14/2022.
//

#ifndef VENGINE3D_VE_CLIP_H
#define VENGINE3D_VE_CLIP_H

#include "ve_core.h"

#include "ve_transform_track.h"
#include "ve_pose.h"

namespace VE {
    class Clip {
    public:
        Clip();
		Clip(const Clip& other) = default;
		Clip(Clip&& other) = default;
		Clip& operator=(const Clip& other) = default;
		Clip& operator=(Clip&& other) = default;
		~Clip() = default;

        float sample(Pose &animatingPose, float time);
        void recalculateDuration();

        TransformTrack &operator[](std::size_t jointIndex);
        std::size_t size();
        std::size_t getJointIndexAtTrack(std::size_t trackIndex);
        void setJointIndexAtTrack(std::size_t trackIndex, std::size_t jointIndex);
        std::string &getName();
        void setName(const std::string name);
        float getDuration();
        float getStartTime();
        float getEndTime();
        bool getLooping();
        void setLooping(bool looping);
    private:
        float agjustTimeToFitRange(float time);

        std::vector<TransformTrack> transformTracks_;
        std::string name_;
        float startTime_;
        float endTime_;
        bool looping_;
    };
}


#endif //VENGINE3D_VE_CLIP_H
