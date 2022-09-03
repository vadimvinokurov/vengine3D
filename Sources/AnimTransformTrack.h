//
// Created by boris on 9/2/2022.
//

#ifndef VENGINE3D_ANIMTRANSFORMTRACK_H
#define VENGINE3D_ANIMTRANSFORMTRACK_H

#include "AnimTrack.h"
#include "Bone.h"

class AnimTransformTrack
{
public:
	void setPositionTrack(const AnimTrack<Vector3>& position);
	void setRotationTrack(const AnimTrack<Quaternion>& rotation);
	void setScalingTrack(const AnimTrack<Vector3>& scale);
	void setBoneId(int32 boneId);
	int32 getBoneId() const;
	void sample(Transform &transform, float time, bool looping) const;

private:
	AnimTrack<Vector3> position_;
	AnimTrack<Quaternion> rotation_;
	AnimTrack<Vector3> scale_;
	int32 boneId_;
};

#endif // VENGINE3D_ANIMTRANSFORMTRACK_H
