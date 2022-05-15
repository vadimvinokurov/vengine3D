//
// Created by boris on 2/13/2022.
//

#ifndef VENGINE3D_VE_TRACK_H
#define VENGINE3D_VE_TRACK_H

#include "ve_core.h"

#include "ve_frame.h"
#include "math/ve_vector.h"
#include "math/ve_quaternion.h"
#include "math/ve_math_utility.h"

namespace VE {
	template<typename T>
	class Track {
	public:
		Track(Interpolation interpolation = Interpolation::Linear);
		Track(std::vector<Frame<T>> frames, Interpolation interpolation = Interpolation::Linear);

		Track(const Track& tack) = default;
		Track(Track&& tack) = default;
		Track& operator=(const Track& tack) = default;
		Track& operator=(Track&& tack) = default;
		~Track() = default;

		Frame<T>& operator[](std::size_t index);
		void setInterpolation(Interpolation interpolation);
		T sample(float time, bool looping) const;
		Interpolation getInterpolation() const;
		float getStartTime() const;
		float getEndTime() const;
		std::size_t size() const;

	private:
		T sampleConstant(float time, bool looping) const;
		T sampleLinear(float time, bool looping) const;
		T sampleCubic(float time, bool looping) const;
		std::size_t frameIndex(float time, bool looping) const;
		float adjustTimeToFitTrack(float time, bool looping) const;
		T hermite(float t, const T& p1, const T& s1, const T& p2, const T& s2) const;
		T normalize_if_quaternion(const T& value) const;

		std::vector<Frame<T>> frames_;
		Interpolation interpolation_;
	};
}  // namespace VE


#endif	//VENGINE3D_VE_TRACK_H
