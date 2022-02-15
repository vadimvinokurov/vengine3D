//
// Created by boris on 2/14/2022.
//

#include "ve_pose.h"

VE::Pose::Pose() {

}

VE::Pose::Pose(unsigned int numJoints) {
    resize(numJoints);
}

VE::Pose::Pose(const VE::Pose &other) {
    *this = other;
}

VE::Pose &VE::Pose::operator=(const VE::Pose &other) {
    if (&other == this) {
        return *this;
    }
    joints_ = other.joints_;
    parents_ = other.parents_;
    return *this;
}

void VE::Pose::resize(unsigned int size) {
    joints_.resize(size);
    parents_.resize(size);
}

void VE::Pose::setParent(unsigned int jointIndex, int parent) {
    parents_[jointIndex] = parent;
}

void VE::Pose::setLocalTransform(unsigned int jointIndex, const VE::Transform &transform) {
    joints_[jointIndex] = transform;
}

unsigned int VE::Pose::jointCount() const {
    return static_cast<unsigned int>(joints_.size());
}

int VE::Pose::getParent(unsigned int jointIndex) const {
    return parents_[jointIndex];
}

const VE::Transform &VE::Pose::getLocalTransform(unsigned int jointIndex) const {
    return joints_[jointIndex];
}

VE::Transform VE::Pose::getGlobalTransform(unsigned int jointIndex) const {
    Transform result = joints_[jointIndex];
    for (int p = parents_[jointIndex]; p >= 0; p = parents_[p]) {
        result = Transform::combine(joints_[p], result);
    }
    return result;
}

void VE::Pose::getMatrixPalette(std::vector<VE::Matrix4> &out) {
    if (out.size() != this->jointCount()) {
        out.resize(this->jointCount());
    }

    for (unsigned int i = 0; i < this->jointCount(); ++i) {
        out[i] = getGlobalTransform(i).toMatrix();
    }
}
