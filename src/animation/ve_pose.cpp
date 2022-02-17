//
// Created by boris on 2/14/2022.
//

#include "ve_pose.h"

VE::Pose::Pose() {

}

VE::Pose::Pose(std::size_t numJoints) {
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
    return *this;
}

void VE::Pose::resize(std::size_t size) {
    joints_.resize(size);
}

void VE::Pose::setParent(std::size_t jointIndex, std::size_t parentIndex) {
    joints_[jointIndex].parentIndex = parentIndex;
}

void VE::Pose::setLocalTransform(std::size_t jointIndex, const VE::Transform &transform) {
    joints_[jointIndex].transform = transform;
}

std::size_t VE::Pose::jointCount() const {
    return joints_.size();
}

std::size_t VE::Pose::getParentIndex(std::size_t jointIndex) const {
    return joints_[jointIndex].parentIndex;
}

const VE::Transform &VE::Pose::getLocalTransform(std::size_t jointIndex) const {
    return joints_[jointIndex].transform;
}

VE::Transform VE::Pose::getGlobalTransform(std::size_t jointIndex) const {
    Transform result = joints_[jointIndex].transform;
    for (std::size_t p = joints_[jointIndex].parentIndex; p != Joint::hasNoParent; p = joints_[p].parentIndex) {
        result = joints_[p].transform * result;
    }
    return result;
}

std::vector<VE::Matrix4> VE::Pose::getMatrixPalette() {
    std::vector<VE::Matrix4> out(this->jointCount());

    for (std::size_t i = 0; i < this->jointCount(); ++i) {
        out[i] = getGlobalTransform(i).toMatrix();
    }

    return out;
}
