//
// Created by boris on 2/13/2022.
//

#include "ve_tarck.h"

using namespace VE;

template
class VE::Track<float, 1>;

template
class VE::Track<Vector3, 3>;

template
class VE::Track<Quaternion, 4>;

template<typename T, unsigned int N>
Track<T, N>::Track() : interpolation_(Interpolation::Linear) {}

template<typename T, unsigned int N>
float Track<T, N>::getStartTime() {
    return frames_.front().time;
}

template<typename T, unsigned int N>
float Track<T, N>::getEndTime() {
    return frames_.back().time;
}

template<typename T, unsigned int N>
T Track<T, N>::sample(float time, bool looping) {
    switch (interpolation_) {
        case Interpolation::Constant:
            return sampleConstant(time, looping);
        case Interpolation::Linear:
            return sampleLinear(time, looping);
        case Interpolation::Cubic:
            return sampleCubic(time, looping);
        default:
            return sampleCubic(time, looping);
    }
}

template<typename T, unsigned int N>
Frame<N> &Track<T, N>::operator[](unsigned int index) {
    return frames_[index];
}

template<typename T, unsigned int N>
void Track<T, N>::resize(unsigned int size) {
    frames_.resize(size);
}

template<typename T, unsigned int N>
unsigned int Track<T, N>::size() {
    return frames_.size();
}

template<typename T, unsigned int N>
Interpolation Track<T, N>::getInterpolation() {
    return interpolation_;
}

template<typename T, unsigned int N>
void Track<T, N>::setInterpolation(Interpolation interpolation) {
    interpolation_ = interpolation;
}

template<typename T, unsigned int N>
T Track<T, N>::hermite(float t, const T &p1, const T &s1, const T &p2, const T &s2) {
    if constexpr (std::is_same<T, Quaternion>::value) {
        if (p1.dot(p2) < 0) {
            return Spline::Hermite(t, p1, s1, p2 * -1.0f, s2).getNormalized();
        } else {
            return Spline::Hermite(t, p1, s1, p2, s2).getNormalized();
        }
    } else {
        return Spline::Hermite(t, p1, s1, p2, s2);
    }
}

template<typename T, unsigned int N>
int Track<T, N>::frameIndex(float time, bool looping) {
    auto size = frames_.size();
    if (size < 2) {
        return -1;
    }

    if (looping) {
        time = loopclamp(time, frames_.front().time, frames_.back().time);
    } else {
        if (time <= frames_.front().time) {
            return 0;
        }
        if (time >= frames_[size - 2].time) {
            return static_cast<int>(size) - 2;
        }
    }

    auto res = std::upper_bound(begin(frames_), end(frames_), time, [](float val, const Frame<N> &frame) {
        return val < frame.time;
    });
    if(res == frames_.begin() || (res == frames_.end() && frames_.back().time > time)) {
        return -1;
    }
    return std::distance(frames_.begin(), res) - 1;

//    for (auto i = size - 1; i >= 0; --i) {
//        if (time >= frames_[i].time) {
//            return static_cast<int>(i);
//        }
//    }
//    return -1;
}

template<typename T, unsigned int N>
float Track<T, N>::adjustTimeToFitTrack(float time, bool looping) {
    auto size = frames_.size();
    if (size < 2) {
        return 0.0f;
    }

    float startTime = frames_.front().time;
    float endTime = frames_.back().time;
    if (endTime - startTime <= 0.0f) {
        return 0.0f;
    }
    return looping ? loopclamp(time, startTime, endTime) : std::clamp(time, startTime, endTime);
}

template<typename T, unsigned int N>
T Track<T, N>::sampleConstant(float time, bool looping) {
    int frame = frameIndex(time, looping);
    if (frame < 0 || frame >= static_cast<int>(frames_.size())) {
        return T();
    }
    return cast(frames_[frame].value);
}

template<typename T, unsigned int N>
T Track<T, N>::sampleLinear(float time, bool looping) {
    int thisFrame = frameIndex(time, looping);
    if (thisFrame < 0 || thisFrame >= static_cast<int>(frames_.size() - 1)) {
        return T();
    }
    int nextFrame = thisFrame + 1;

    float trackTime = adjustTimeToFitTrack(time, looping);
    float thisTime = frames_[thisFrame].time;
    float frameDelta = frames_[nextFrame].time - thisTime;
    if (frameDelta <= 0.0f) {
        return T();
    }
    float t = (trackTime - thisTime) / frameDelta;
    T p1 = cast(frames_[thisFrame].value);
    T p2 = cast(frames_[nextFrame].value);
    return linearInterpolate(p1, p2, t);
}

template<typename T, unsigned int N>
T Track<T, N>::sampleCubic(float time, bool looping) {
    int thisFrame = frameIndex(time, looping);
    if (thisFrame < 0 || thisFrame >= static_cast<int>(frames_.size() - 1)) {
        return T();
    }
    int nextFrame = thisFrame + 1;

    float trackTime = adjustTimeToFitTrack(time, looping);
    float thisTime = frames_[thisFrame].time;
    float frameDelta = frames_[nextFrame].time - thisTime;
    if (frameDelta <= 0.0f) {
        return T();
    }
    float t = (trackTime - thisTime) / frameDelta;

    T p1 = cast(frames_[thisFrame].value);
    T s1;
    memcpy(&s1, frames_[thisFrame].out, N * sizeof(float));
    s1 *= frameDelta;
    T p2 = cast(frames_[nextFrame].value);
    T s2;
    memcpy(&s2, frames_[nextFrame].in, N * sizeof(float));
    s2 *= frameDelta;
    return hermite(t, p1, s1, p2, s2);
}

template<>
float Track<float, 1>::cast(const float *value) {
    return value[0];
}

template<>
Vector3 Track<Vector3, 3>::cast(const float *value) {
    return Vector3(value);
}

template<>
Quaternion Track<Quaternion, 4>::cast(const float *value) {
    return Quaternion(value).normalize();
}
