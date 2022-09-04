//
// Created by boris on 8/23/2022.
//

#include "Skeleton.h"
Skeleton::Skeleton(const std::vector<Bone> &bones) : bones_(bones)
{
}

Transform Skeleton::getGlobalTransform(int32 boneId) const
{
	Transform result = bones_[boneId].transform * bones_[boneId].offset;
	while (bones_[boneId].parentId != INVALID_BODE_ID)
	{
		boneId = bones_[boneId].parentId;
		result = bones_[boneId].transform * result;
	}
	return globalInverseTransform_ * result;
}

std::vector<Matrix4> Skeleton::getMatrixPalette() const
{
	std::vector<Matrix4> result(bones_.size());

	for (int32 boneId = 0; boneId < bones_.size(); ++boneId)
	{
		result[boneId] = getGlobalTransform(boneId).toMatrix();
	}
	return result;
}

Transform &Skeleton::operator[](int32 boneId)
{
	return bones_[boneId].transform;
}

const Transform &Skeleton::operator[](int32 boneId) const
{
	return bones_[boneId].transform;
}
