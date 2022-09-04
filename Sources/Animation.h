//
// Created by boris on 9/3/2022.
//

#ifndef VENGINE3D_ANIMATION_H
#define VENGINE3D_ANIMATION_H

#include "AnimTransformTrack.h"
#include "Skeleton.h"

class Animation
{
public:
	Animation(const std::string& name, float durationInSeconds);
	void addTransformTrack(const AnimTransformTrack &transformTrack);
	void setLooping(bool looping);
	float sample(Skeleton &skeleton, float time) const;

private:
	float adjustTimeToFitAnimation(float time) const;
	std::vector<AnimTransformTrack> transformTracks_;
	std::string name_;
	bool looping_ = false;
	float durationInSeconds_ = 0;
};

#endif // VENGINE3D_ANIMATION_H
