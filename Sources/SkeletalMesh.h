//
// Created by boris on 8/23/2022.
//

#ifndef VENGINE3D_SKELETALMESH_H
#define VENGINE3D_SKELETALMESH_H
#include "spdlog/spdlog.h"
#include "SkeletalMeshElement.h"
#include "Material.h"

class SkeletalMesh
{
public:
	void addSkeletalMeshElement(const std::shared_ptr<SkeletalMeshElement>& skeletalMeshElement);

	std::vector<std::shared_ptr<SkeletalMeshElement>> skeletalMeshElements;
	std::vector<std::shared_ptr<Material>> materials;
};

#endif // VENGINE3D_SKELETALMESH_H
