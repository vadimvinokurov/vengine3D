//
// Created by boris on 2/14/2022.
//

#include "ve_clip.h"

VE::Clip::Clip() {
    name_ = "No name";
    startTime_ = 0.0f;
    endTime_ = 0.0f;
    looping_ = true;
}

float VE::Clip::sample(VE::Pose &outPose, float time) {
    if (getDuration() == 0.0f) {
        return 0.0f;
    }
    time = agjustTimeToFitRange(time);

    for (const auto &transformTrack: transformTracks_) {
        size_t jointIndex = transformTrack.getJointIndex();
        Transform local = outPose.getLocalTransform(jointIndex);
        Transform animated = transformTrack.sample(local, time, looping_);
        outPose.setLocalTransform(jointIndex, animated);
    }
    return time;
}

void VE::Clip::recalculateDuration() {
    startTime_ = 0.0f;
    endTime_ = 0.0f;
    bool startSet = false;
    bool endSet = false;
    for (const auto &track: transformTracks_) {
        if (track.isValid()) {
            float startTime = track.getStartTime();
            float endTime = track.getEndTime();

            if (startTime < startTime_ || !startSet) {
                startTime_ = startTime;
                startSet = true;
            }

            if (endTime > endTime_ || !endSet) {
                endTime_ = endTime;
                endSet = true;
            }
        }
    }
}

VE::TransformTrack &VE::Clip::operator[](std::size_t jointIndex) {
    for (auto &track: transformTracks_) {
        if (track.getJointIndex() == jointIndex) {
            return track;
        }
    }
    transformTracks_.push_back(TransformTrack());
    transformTracks_.back().setJointIndex(jointIndex);
    return transformTracks_.back();
}

std::size_t VE::Clip::size() {
    return transformTracks_.size();
}

std::size_t VE::Clip::getJointIndexAtTrack(std::size_t trackIndex) {
    return transformTracks_[trackIndex].getJointIndex();
}

void VE::Clip::setJointIndexAtTrack(std::size_t trackIndex, std::size_t jointIndex) {
    transformTracks_[trackIndex].setJointIndex(jointIndex);
}

std::string &VE::Clip::getName() {
    return name_;
}

void VE::Clip::setName(std::string name) {
    name_ = name;
}

float VE::Clip::getDuration() {
    return endTime_ - startTime_;
}

float VE::Clip::getStartTime() {
    return startTime_;
}

float VE::Clip::getEndTime() {
    return endTime_;
}

bool VE::Clip::getLooping() {
    return looping_;
}

void VE::Clip::setLooping(bool looping) {
    looping_ = looping;
}

float VE::Clip::agjustTimeToFitRange(float time) {
    return looping_ ? loopclamp(time, startTime_, endTime_) : std::clamp(time, startTime_, endTime_);
}
