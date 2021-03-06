//
// Created by boris on 2/13/2022.
//

#include "ve_transform_track.h"

using namespace VE;

void VE::TransformTrack::setJointIndex(std::size_t jointIndex) {
    jointIndex_ = jointIndex;
}

std::size_t VE::TransformTrack::getJointIndex() const {
    return jointIndex_;
}

float VE::TransformTrack::getStartTime() const {
    float result = 0.0f;
    bool isSet = false;
    if (position.size() > 1) {
        result = position.getStartTime();
        isSet = true;
    }
    if (float rotationStart = rotation.getStartTime(); rotation.size() > 1) {
        if (rotationStart < result || !isSet) {
            result = rotationStart;
            isSet = true;
        }
    }
    if (float scaleStart = scale.getStartTime(); scale.size() > 1) {
        if (scaleStart < result || !isSet) {
            result = scaleStart;
        }
    }
    return result;
}

float VE::TransformTrack::getEndTime() const {
    float result = 0.0f;
    bool isSet = false;
    if (position.size() > 1) {
        result = position.getEndTime();
        isSet = true;
    }
    if (float rotationEnd = rotation.getEndTime(); rotation.size() > 1) {
        if (rotationEnd > result || !isSet) {
            result = rotationEnd;
            isSet = true;
        }
    }
    if (float scaleEnd = scale.getEndTime(); scale.size() > 1) {
        if (scaleEnd > result || !isSet) {
            result = scaleEnd;
        }
    }
    return result;
}

bool VE::TransformTrack::isValid() const {
    return position.size() > 1 || rotation.size() > 1 || scale.size() > 1;
}

void VE::TransformTrack::sample(VE::Transform &transform, float time, bool looping) const {
    if (position.size() > 1) {
		transform.position = position.sample(time, looping);
    }
    if (rotation.size() > 1) {
		transform.rotation = rotation.sample(time, looping);
    }
    if (scale.size() > 1) {
		transform.scale = scale.sample(time, looping);
    }
}

