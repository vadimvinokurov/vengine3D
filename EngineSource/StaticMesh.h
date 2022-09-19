//
// Created by boris on 8/20/2022.
//

#ifndef VENGINE3D_STATICMESH_H
#define VENGINE3D_STATICMESH_H

#include <vector>
#include "Math/Vector.h"
struct StaticMesh
{
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> textureCoordinates;
	std::vector<uint32> indices;
};

#endif // VENGINE3D_STATICMESH_H
