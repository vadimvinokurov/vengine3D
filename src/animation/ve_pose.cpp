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

unsigned int VE::Pose::size() {
    return joints_.size();
}

int VE::Pose::getParent(unsigned int index) {
    return parents_[index];
}

void VE::Pose::setParent(unsigned int index, int parent) {
    parents_[index] = parent;
}

VE::Transform VE::Pose::getLocalTransform(unsigned int index) {
    return joints_[index];
}

void VE::Pose::setLocalTransform(unsigned int index, const VE::Transform &transform) {
    joints_[index] = transform;
}

VE::Transform VE::Pose::getGlobalTransform(unsigned int index) {
    Transform result = joints_[index];
    for (int p = parents_[index]; p >= 0; p = parents_[p]) {
        result = Transform::combine(joints_[p], result);
    }
    return result;
}

VE::Transform VE::Pose::operator=(unsigned int index) {
    return getGlobalTransform(index);
}

void VE::Pose::getMatrixPalette(std::vector<VE::Matrix4> &out) {
    if (out.size() != this->size()) {
        out.resize(this->size());
    }

    for (unsigned int i = 0; i < this->size(); ++i) {
        out[i] = getGlobalTransform(i).toMatrix();
    }
}

bool VE::Pose::operator==(const VE::Pose &other) {
    if (joints_.size() != other.joints_.size()) {
        return false;
    }
    if (parents_.size() != other.parents_.size()) {
        return false;
    }

    for (size_t i = 0; i < joints_.size(); ++i) {
        Transform thisLocal = joints_[i];
        Transform otherLocal = other.joints_[i];

        if (parents_[i] != other.parents_[i]) {
            return false;
        }
        if (thisLocal.position != otherLocal.position) {
            return false;
        }
        if (thisLocal.rotation != otherLocal.rotation) {
            return false;
        }
        if (thisLocal.scale != otherLocal.scale) {
            return false;
        }
    }
    return true;
}

bool VE::Pose::operator!=(const VE::Pose &other) {
    return !(*this == other);
}
