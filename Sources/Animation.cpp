//
// Created by boris on 9/3/2022.
//

#include <algorithm>
#include "Animation.h"

void Animation::addTransformTrack(const AnimTransformTrack &transformTrack)
{
	transformTracks_.push_back(transformTrack);
}

void Animation::setLooping(bool looping)
{
	looping_ = looping;
}

void Animation::setDuration(float durationInSeconds)
{
	durationInSeconds_ = durationInSeconds;
}

float Animation::sample(Skeleton &skeleton, float time) const
{
	time = adjustTimeToFitAnimation(time);

	for (const auto &transformTrack : transformTracks_)
	{
		transformTrack.sample(skeleton[transformTrack.getBoneId()], time, looping_);
	}
	return time;
}

float Animation::adjustTimeToFitAnimation(float time) const
{
	return looping_ ? VEMath::loopclamp(time, 0.0f, durationInSeconds_) : std::clamp(time, 0.0f, durationInSeconds_);
}
void Animation::setName(const std::string &name)
{
	name_ = name;
}
