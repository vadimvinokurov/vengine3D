//
// Created by boris on 2/14/2022.
//

#ifndef VENGINE3D_VE_POSE_H
#define VENGINE3D_VE_POSE_H

#include "stdlibraries.h"
#include "math\ve_transform.h"

namespace VE {
    class Pose {
    public:
        Pose();
        Pose(unsigned int numJoints);
        Pose(const Pose &other);
        Pose &operator=(const Pose &other);
        void resize(unsigned int size);
        void setParent(unsigned int jointIndex, int parent);
        void setLocalTransform(unsigned int jointIndex, const Transform &transform);

        unsigned int jointCount() const;
        int getParent(unsigned int jointIndex) const ;
        const Transform& getLocalTransform(unsigned int jointIndex) const;
        Transform getGlobalTransform(unsigned int jointIndex) const;
        void getMatrixPalette(std::vector<Matrix4> &out);
    private:
        std::vector<Transform> joints_;
        std::vector<int> parents_;
    };
}


#endif //VENGINE3D_VE_POSE_H
