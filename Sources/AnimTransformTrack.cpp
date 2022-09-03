//
// Created by boris on 9/2/2022.
//

#include "AnimTransformTrack.h"
void AnimTransformTrack::setPositionTrack(const AnimTrack<Vector3> &position)
{
	position_ = position;
}

void AnimTransformTrack::setRotationTrack(const AnimTrack<Quaternion> &rotation)
{
	rotation_ = rotation;
}

void AnimTransformTrack::setScalingTrack(const AnimTrack<Vector3> &scale)
{
	scale_ = scale;
}

void AnimTransformTrack::setBoneId(int32 boneId)
{
	boneId_ = boneId;
}
int32 AnimTransformTrack::getBoneId() const
{
	return boneId_;
}
void AnimTransformTrack::sample(Transform &transform, float time, bool looping) const
{
	if (!position_.empty())
	{
		transform.position = position_.sample(time, looping);
	}
	if (!rotation_.empty())
	{
		transform.rotation = rotation_.sample(time, looping);
	}
	if (!scale_.empty())
	{
		transform.scale = scale_.sample(time, looping);
	}
}
