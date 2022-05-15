//
// Created by boris on 2/20/2022.
//

#include "ve_skeleton.h"
using namespace VE;
VE::Skeleton::Skeleton() {}

VE::Skeleton::Skeleton(const VE::Pose& rest, const VE::Pose& bind, const std::vector<std::string>& names) {
    set(rest, bind, names);
}

void VE::Skeleton::set(const VE::Pose& rest, const VE::Pose& bind, const std::vector<std::string>& names) {
	pose_ = rest;
    bindPose_ = bind;
    jointNames_ = names;

	updateInvBindPose();
}

const VE::Pose& VE::Skeleton::getBindPose() const {
    return bindPose_;
}

Pose& VE::Skeleton::pose() {
    return pose_;
}

const Pose& Skeleton::pose() const {
	return pose_;
}

const std::vector<Matrix4>& VE::Skeleton::getInvBindPose() const {
    return invBindPose_;
}

const std::vector<std::string>& VE::Skeleton::getJointNames() const {
    return jointNames_;
}
const std::string& Skeleton::getJointName(std::size_t index) const {
    return jointNames_[index];
}

void VE::Skeleton::updateInvBindPose() {
    auto jointCount = bindPose_.jointsCount();
    invBindPose_.resize(jointCount);
    for(std::size_t i = 0; i < jointCount; ++i){
        invBindPose_[i] = bindPose_.getGlobalTransform(i).getInversed().toMatrix();
    }
}
