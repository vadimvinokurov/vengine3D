//
// Created by boris on 8/23/2022.
//

#ifndef VENGINE3D_SKELETALMESHDATA_H
#define VENGINE3D_SKELETALMESHDATA_H

#include <vector>
#include "Math/Vector.h"

struct SkeletalMeshSection
{
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> textureCoordinates;
	std::vector<uint32> indices;
	std::vector<IVector4> influenceBoneIndices;
	std::vector<Vector4> influenceBoneWeight;
};

#endif // VENGINE3D_SKELETALMESHDATA_H
