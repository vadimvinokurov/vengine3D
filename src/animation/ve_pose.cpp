//
// Created by boris on 2/14/2022.
//

#include "ve_pose.h"

void VE::Pose::addJoint(std::size_t jointIndex, const VE::Transform& transform, size_t parentIndex) {
	if(jointIndex >= joints_.size()) joints_.resize(jointIndex + 1);
	joints_[jointIndex] = Joint{transform, parentIndex};
}

VE::Transform& VE::Pose::operator[](std::size_t jointIndex) {
	return joints_.at(jointIndex).transform;
}

const VE::Transform& VE::Pose::operator[](std::size_t jointIndex) const {
	return joints_.at(jointIndex).transform;
}

std::size_t VE::Pose::getParentIndex(std::size_t jointIndex) const {
	return joints_.at(jointIndex).parentIndex;
}

VE::Transform VE::Pose::getGlobalTransform(std::size_t jointIndex) const {
	Transform result = joints_.at(jointIndex).transform;
	for (std::size_t p = joints_.at(jointIndex).parentIndex; p != Joint::hasNoParent; p = joints_.at(p).parentIndex) { result = joints_.at(p).transform * result; }
	return result;
}

std::vector<VE::Matrix4> VE::Pose::getMatrixPalette() const {
	std::vector<VE::Matrix4> out(joints_.size());
	for (std::size_t i = 0; i < joints_.size(); ++i) { out[i] = getGlobalTransform(i).toMatrix(); }
	return out;
}

std::size_t VE::Pose::jointsCount() const {
	return joints_.size();
}