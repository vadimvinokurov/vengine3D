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
        unsigned int size();
        int getParent(unsigned int index);
        void setParent(unsigned int index, int parent);
        Transform getLocalTransform(unsigned int index);
        void setLocalTransform(unsigned int index, const Transform &transform);

        Transform getGlobalTransform(unsigned int index);
        Transform operator=(unsigned int index);
        void getMatrixPalette(std::vector<Matrix4> &out);

        bool operator==(const Pose &other);
        bool operator!=(const Pose &other);
    private:
        std::vector<Transform> joints_;
        std::vector<int> parents_;
    };
}


#endif //VENGINE3D_VE_POSE_H
