//
// Created by boris on 8/23/2022.
//

#ifndef VENGINE3D_SKELETON_H
#define VENGINE3D_SKELETON_H

#include "Bone.h"
#include <unordered_map>

class Skeleton
{
public:
	explicit Skeleton(const std::vector<Bone> &bones);
	Transform& operator[](int32 boneId);
	const Transform& operator[](int32 boneId) const;
	Transform getGlobalTransform(int32 boneId) const;
	std::vector<Matrix4> getMatrixPalette() const;
	std::vector<Matrix4> getInvMatrixPalette() const;
private:
	std::vector<Bone> bones_;
};

#endif // VENGINE3D_SKELETON_H
