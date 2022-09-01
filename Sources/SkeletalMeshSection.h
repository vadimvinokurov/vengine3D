//
// Created by boris on 8/23/2022.
//

#ifndef VENGINE3D_SKELETALMESHDATA_H
#define VENGINE3D_SKELETALMESHDATA_H

#include <vector>
#include "Math/Vector.h"
#include "Bone.h"

struct SkeletalMeshSection
{
	void addBoneInfluence(uint32 vertexIndex, int32 boneId, float weight){
		for(int32 i = 0; i < 4; ++i) {
			if(influenceBones[vertexIndex].v[i] == INVALID_BODE_ID) {
				influenceBones[vertexIndex].v[i] = boneId;
				influenceWeights[vertexIndex].v[i] = weight;
				return;
			}
		}
		//assert(0);
	}
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> textureCoordinates;
	std::vector<uint32> indices;
	std::vector<IVector4> influenceBones;
	std::vector<Vector4> influenceWeights;
};

#endif // VENGINE3D_SKELETALMESHDATA_H
