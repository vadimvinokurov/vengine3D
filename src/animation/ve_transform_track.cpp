//
// Created by boris on 2/13/2022.
//

#include "ve_transform_track.h"

using namespace VE;

VE::TransformTrack::TransformTrack() {
    id_ = 0;
}


void VE::TransformTrack::setId(unsigned int id) {
    id_ = id;
}

unsigned int VE::TransformTrack::getId() {
    return id_;
}

VE::Track<Vector3> &VE::TransformTrack::getPositionTrack() {
    return position_;
}

VE::Track<Quaternion> &VE::TransformTrack::getRotationTrack() {
    return rotation_;
}

VE::Track<Vector3> &VE::TransformTrack::getScaleTrack() {
    return scale_;
}

float VE::TransformTrack::getStartTime() {
    float result = 0.0f;
    bool isSet = false;
    if (position_.size() > 1) {
        result = position_.getStartTime();
        isSet = true;
    }
    if (rotation_.size() > 1) {
        float rotationStart = rotation_.getStartTime();
        if (rotationStart < result || !isSet) {
            result = rotationStart;
            isSet = true;
        }
    }
    if (scale_.size() > 1) {
        float scaleStart = scale_.getStartTime();
        if (scaleStart < result || !isSet) {
            result = scaleStart;
        }
    }
    return result;
}

float VE::TransformTrack::getEndTime() {
    float result = 0.0f;
    bool isSet = false;
    if (position_.size() > 1) {
        result = position_.getEndTime();
        isSet = true;
    }
    if (rotation_.size() > 1) {
        float rotationEnd = rotation_.getEndTime();
        if (rotationEnd > result || !isSet) {
            result = rotationEnd;
            isSet = true;
        }
    }
    if (scale_.size() > 1) {
        float scaleEnd = scale_.getEndTime();
        if (scaleEnd > result || !isSet) {
            result = scaleEnd;
        }
    }
    return result;
}

bool VE::TransformTrack::isValid() {
    return position_.size() > 1 ||
           rotation_.size() > 1 ||
           scale_.size() > 1;
}

VE::Transform VE::TransformTrack::sample(const VE::Transform &ref, float time, bool looping) {
    Transform result = ref;
    if (position_.size() > 1) {
        result.position = position_.sample(time, looping);
    }
    if (rotation_.size() > 1) {
        result.rotation = rotation_.sample(time, looping);
    }
    if (scale_.size() > 1) {
        result.scale = scale_.sample(time, looping);
    }
    return result;
}

