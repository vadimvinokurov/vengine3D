//
// Created by boris on 2/20/2022.
//

#ifndef VENGINE3D_SRC_ANIMATION_VE_SKELETON_H_
#define VENGINE3D_SRC_ANIMATION_VE_SKELETON_H_
#include "ve_core.h"
#include "ve_pose.h"

namespace VE {
    class Skeleton {
    public:
        Skeleton();
        Skeleton(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);
        void set(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);

        const Pose& getBindPose() const;
        const Pose& getRestPose() const;

        const std::vector<Matrix4>& getInvBindPose() const;
        const std::vector<std::string>& getJointNames() const;
        const std::string& getJointName(std::size_t index) const;

    private:
        void updateInvBindPose();

        Pose restPose_;
        Pose bindPose_;
        std::vector<Matrix4> invBindPose_;
        std::vector<std::string> jointNames_;
    };
}  // namespace VE


#endif  //VENGINE3D_SRC_ANIMATION_VE_SKELETON_H_
