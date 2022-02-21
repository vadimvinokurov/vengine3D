//
// Created by boris on 2/14/2022.
//

#ifndef VENGINE3D_VE_POSE_H
#define VENGINE3D_VE_POSE_H

#include "ve_core.h"

#include "math/ve_transform.h"

namespace VE {
    struct Joint{
        static constexpr std::size_t hasNoParent = -1;

        Transform transform;
        std::size_t parentIndex;
    };

    class Pose {
    public:
        Pose();
        Pose(std::size_t numJoints);
        Pose(const Pose &other);
        Pose &operator=(const Pose &other);
        void resize(std::size_t size);
        void setParent(std::size_t jointIndex, std::size_t parentIndex);
        void setLocalTransform(std::size_t jointIndex, const Transform &transform);

        std::size_t jointsCount() const;
        std::size_t getParentIndex(std::size_t jointIndex) const;
        const Transform &getLocalTransform(std::size_t jointIndex) const;
        Transform getGlobalTransform(std::size_t jointIndex) const;
        std::vector<VE::Matrix4> getMatrixPalette() const;
    private:
        std::vector<Joint> joints_;
    };
}


#endif //VENGINE3D_VE_POSE_H
