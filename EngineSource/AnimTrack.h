//
// Created by boris on 9/2/2022.
//

#ifndef VENGINE3D_ANIMTRACK_H
#define VENGINE3D_ANIMTRACK_H

#include <sstream>
#include "AnimFrame.h"
#include "Math/MathUtils.h"
#include <algorithm>

template <typename T>
class AnimTrack
{
public:
	AnimTrack() = default;
	explicit AnimTrack(const std::vector<AnimFrame<T>> &frames) : frames_(frames){};
	T sample(float time, bool looping) const;
	uint32 frameIndex(float time, bool looping) const;
	float adjustTimeToFitTrack(float time, bool looping) const;
	bool empty() const;

private:
	T normalize_if_quaternion(const T &value) const;

	std::vector<AnimFrame<T>> frames_;
};


template <typename T>
T AnimTrack<T>::sample(float time, bool looping) const
{
	if (empty())
	{
		return T();
	}
	float itime = time;
	time = adjustTimeToFitTrack(time, looping);
	uint32 thisFrame = frameIndex(time, looping);
	if (thisFrame == frames_.size() - 1)
	{
		return frames_[thisFrame].value;
	}
	uint32 nextFrame = thisFrame + 1;

	float startSampleTime = frames_[thisFrame].time;
	float endSampleTime = frames_[nextFrame].time;
	float deltaTime = endSampleTime - startSampleTime;
	float t = (time - startSampleTime) / deltaTime;

	T p1 = normalize_if_quaternion(frames_[thisFrame].value);
	T p2 = normalize_if_quaternion(frames_[nextFrame].value);

	return VEMath::linearInterpolate(p1, p2, t);
}

template <typename T>
float AnimTrack<T>::adjustTimeToFitTrack(float time, bool looping) const
{
	float startTime = frames_.front().time;
	float endTime = frames_.back().time;
	return looping ? VEMath::loopclamp(time, startTime, endTime) : std::clamp(time, startTime, endTime);
}

template <typename T>
uint32 AnimTrack<T>::frameIndex(float time, bool looping) const
{
	if (time <= frames_.front().time)
	{
		return 0;
	}
	if (time >= frames_.back().time)
	{
		return frames_.size() - 1;
	}

	for (uint32 i = frames_.size() - 1; i >= 0; --i)
	{
		if (frames_[i].time <= time)
		{
			return i;
		}
	}
	assert(false && "Incorrect frame index");
	return 0;
}
template <typename T>
T AnimTrack<T>::normalize_if_quaternion(const T &value) const
{
	if constexpr (std::is_same_v<T, Quaternion>)
	{
		return value.getNormalized();
	}
	else
	{
		return value;
	}
}

template <typename T>
bool AnimTrack<T>::empty() const
{
	return frames_.size() < 2;
}

#endif // VENGINE3D_ANIMTRACK_H
