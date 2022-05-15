//
// Created by boris on 2/14/2022.
//

#ifndef VENGINE3D_VE_POSE_H
#define VENGINE3D_VE_POSE_H

#include "ve_core.h"

#include "math/ve_transform.h"

namespace VE {
	struct Joint {
		static constexpr std::size_t hasNoParent = -1;
		Transform transform;
		std::size_t parentIndex;
	};

	class Pose {
	public:
		Pose() = default;
		Pose(const Pose& other) = default;
		Pose(Pose&& other) = default;
		Pose& operator=(const Pose& other) = default;
		Pose& operator=(Pose&& other) = default;
		~Pose() = default;

		void addJoint(std::size_t jointIndex, const Transform& transform, size_t parentIndex = Joint::hasNoParent);
		std::size_t jointsCount() const;
		Transform& operator[](std::size_t jointIndex);
		const Transform& operator[](std::size_t jointIndex) const;
		std::size_t getParentIndex(std::size_t jointIndex) const;
		Transform getGlobalTransform(std::size_t jointIndex) const;
		std::vector<Matrix4> getMatrixPalette() const;

	private:
		std::vector<Joint> joints_;
	};
}  // namespace VE


#endif	//VENGINE3D_VE_POSE_H
