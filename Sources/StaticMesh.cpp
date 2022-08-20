//
// Created by boris on 8/20/2022.
//

#include "StaticMesh.h"
StaticMesh::StaticMesh()
{
	vertices = {Vector3(1.0f, 1.0f, 0.0f),	Vector3(-1.0f, 1.0f, 0.0f),
				Vector3(1.0f, -1.0f, 0.0f), Vector3(1.0f, -1.0f, 0.0f),
				Vector3(-1.0f, 1.0f, 0.0f), Vector3(-1.0f, -1.0f, 0.0f)};
	normals = std::vector<Vector3>(6, Vector3(0.0f, 0.0f, 1.0f));

	textureCoordinates = {Vector2(1.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f),
						  Vector2(1.0f, 1.0f), Vector2(0.0f, 0.0f), Vector2(0.0f, 1.0f)};

	indices = {0, 1, 2, 3, 4, 5};
}
