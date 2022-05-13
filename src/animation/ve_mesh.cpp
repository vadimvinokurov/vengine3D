//
// Created by boris on 2/20/2022.
//

#include "ve_mesh.h"
using namespace VE;

Mesh::Mesh(const Mesh& other) {}

Mesh& Mesh::operator=(const Mesh& other) {
	return *this;
}

void Mesh::updateOpenGLBuffers() {
	positionsGPU.set(positions);
	normalsGPU.set(normals);
	textureCoordinatesGPU.set(textureCoordinates);
	jointsInfluencesWeightsGPU.set(jointsInfluencesWeights);
	jointsInfluencesGPU.set(jointsInfluences);
	indicesGPU.set(indices);
}

void Mesh::skin(const Skeleton& skeleton, const Pose& pose) {
	if (positions.empty()) return;
	skinnedPosition_.resize(positions.size());
	skinnedNormals_.resize(positions.size());

	auto posePalette = pose.getMatrixPalette();
	auto invPosePalette = skeleton.getInvBindPose();

	for (std::size_t i = 0; i < positions.size(); ++i) {
		IVector4& j = jointsInfluences[i];
		Vector4& w = jointsInfluencesWeights[i];

		auto m0 = (posePalette[j.x] * invPosePalette[j.x]) * w.x;
		auto m1 = (posePalette[j.y] * invPosePalette[j.y]) * w.y;
		auto m2 = (posePalette[j.z] * invPosePalette[j.z]) * w.z;
		auto m3 = (posePalette[j.w] * invPosePalette[j.w]) * w.w;

		auto skinMatrix = m0 + m1 + m2 + m3;

        Transform t1(Quaternion::fromAxisAngle(Vector3(1, 0, 0), M_PI / 2));
        Transform t2(Quaternion::fromAxisAngle(Vector3(0, 0, 1), M_PI ));
        skinMatrix = (t2*t1).toMatrix() * skinMatrix;

		skinnedPosition_[i] = skinMatrix.transformPoint(positions[i]);
		skinnedNormals_[i] = skinMatrix.transformVector(normals[i]);
	}


	positionsGPU.set(skinnedPosition_);
	normalsGPU.set(skinnedNormals_);
}
