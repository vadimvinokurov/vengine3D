//
// Created by boris on 2/13/2022.
//

#include "ve_transform_track.h"

using namespace VE;

VE::TransformTrack::TransformTrack() {
    jointId_ = 0;
}

void VE::TransformTrack::setJointId(unsigned int jointId) {
    jointId_ = jointId;
}

unsigned int VE::TransformTrack::getJointId() const {
    return jointId_;
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

VE::Transform VE::TransformTrack::sample(const VE::Transform &ref, float time, bool looping) const {
    Transform result = ref;
    if (position.size() > 1) {
        result.position = position.sample(time, looping);
    }
    if (rotation.size() > 1) {
        result.rotation = rotation.sample(time, looping);
    }
    if (scale.size() > 1) {
        result.scale = scale.sample(time, looping);
    }
    return result;
}

