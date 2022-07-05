//
// Created by boris on 2/13/2022.
//

#include "ve_track.h"

using namespace VE;

template class VE::Track<Vector3>;
template class VE::Track<Quaternion>;

template<typename T>
Track<T>::Track(Interpolation interpolation) : interpolation_(interpolation) {}

template<typename T>
Track<T>::Track(std::vector<Frame<T>> frames, Interpolation interpolation) : frames_(std::move(frames)),
																			 interpolation_(interpolation) {}

template<typename T>
Frame<T>& Track<T>::operator[](std::size_t index) {
	return frames_[index];
}

template<typename T>
void Track<T>::setInterpolation(Interpolation interpolation) {
	interpolation_ = interpolation;
}

template<typename T>
T Track<T>::sample(float time, bool looping) const {
	switch (interpolation_) {
		case Interpolation::Constant: return sampleConstant(time, looping);
		case Interpolation::Linear: return sampleLinear(time, looping);
		case Interpolation::Cubic: return sampleCubic(time, looping);
		default: return sampleConstant(time, looping);
	}
}

template<typename T>
float Track<T>::getStartTime() const {
	return frames_.empty() ? 0.0f : frames_.front().time;
}

template<typename T>
float Track<T>::getEndTime() const {
	return frames_.empty() ? 0.0f : frames_.back().time;
}

template<typename T>
std::size_t Track<T>::size() const {
	return frames_.size();
}

template<typename T>
Interpolation Track<T>::getInterpolation() const {
	return interpolation_;
}

template<typename T>
T Track<T>::sampleConstant(float time, bool looping) const {
	auto frame = frameIndex(time, looping);
	if (frame >= frames_.size()) { return T(); }
	return normalize_if_quaternion(frames_[frame].value);
}

template<typename T>
T Track<T>::sampleLinear(float time, bool looping) const {
	auto thisFrame = frameIndex(time, looping);
	if (frames_.empty() || thisFrame >= frames_.size() - 1) { return T(); }
	auto nextFrame = thisFrame + 1;

	float trackTime = adjustTimeToFitTrack(time, looping);
	float frameDelta = frames_[nextFrame].time - frames_[thisFrame].time;
	if (frameDelta <= 0.0f) { return T(); }
	float t = (trackTime - frames_[thisFrame].time) / frameDelta;

	T p1 = normalize_if_quaternion(frames_[thisFrame].value);
	T p2 = normalize_if_quaternion(frames_[nextFrame].value);

	return linearInterpolate(p1, p2, t);
}

template<typename T>
T Track<T>::sampleCubic(float time, bool looping) const {
	auto thisFrame = frameIndex(time, looping);
	if (frames_.empty() || thisFrame >= frames_.size() - 1) { return T(); }
	auto nextFrame = thisFrame + 1;

	float trackTime = adjustTimeToFitTrack(time, looping);
	float frameDelta = frames_[nextFrame].time - frames_[thisFrame].time;
	if (frameDelta <= 0.0f) { return T(); }
	float t = (trackTime - frames_[thisFrame].time) / frameDelta;

	T p1 = normalize_if_quaternion(frames_[thisFrame].value);
	T p2 = normalize_if_quaternion(frames_[nextFrame].value);
	T s1 = frames_[thisFrame].out * frameDelta;
	T s2 = frames_[nextFrame].in * frameDelta;

	return hermite(t, p1, s1, p2, s2);
}

template<typename T>
std::size_t Track<T>::frameIndex(float time, bool looping) const {
	auto size = frames_.size();
	if (size < 2) { return size; }

	if (looping) {
		time = loopclamp(time, frames_.front().time, frames_.back().time);
	} else {
		if (time <= frames_.front().time) { return 0; }
		if (time >= frames_[size - 2].time) { return size - 2; }
	}

	auto res = std::upper_bound(begin(frames_), end(frames_), time, [](float val, const Frame<T>& frame) { return val < frame.time; });
	if (res == frames_.begin() || (res == frames_.end() && frames_.back().time > time)) { return size; }
	return std::distance(frames_.begin(), res) - 1;
}

template<typename T>
float Track<T>::adjustTimeToFitTrack(float time, bool looping) const {
	auto size = frames_.size();
	if (size < 2) { return 0.0f; }

	float startTime = frames_.front().time;
	float endTime = frames_.back().time;
	if (endTime - startTime <= 0.0f) { return 0.0f; }
	return looping ? loopclamp(time, startTime, endTime) : std::clamp(time, startTime, endTime);
}

template<>
Vector3 Track<Vector3>::hermite(float t, const Vector3& p1, const Vector3& s1, const Vector3& p2, const Vector3& s2) const {
	return Spline::Hermite(t, p1, s1, p2, s2);
}


template<>
Quaternion Track<Quaternion>::hermite(float t, const Quaternion& p1, const Quaternion& s1, const Quaternion& p2, const Quaternion& s2) const {
	if (p1.dot(p2) < 0) {
		return Spline::Hermite(t, p1, s1, p2 * -1.0f, s2).getNormalized();
	} else {
		return Spline::Hermite(t, p1, s1, p2, s2).getNormalized();
	}
}

template<>
Vector3 Track<Vector3>::normalize_if_quaternion(const Vector3& value) const {
	return value;
}

template<>
Quaternion Track<Quaternion>::normalize_if_quaternion(const Quaternion& value) const {
	return value.getNormalized();
}