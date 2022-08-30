//
// Created by boris on 8/23/2022.
//

#ifndef VENGINE3D_SKELETALMESH_H
#define VENGINE3D_SKELETALMESH_H
#include "spdlog/spdlog.h"
#include "SkeletalMeshModel.h"
#include "Material.h"

class SkeletalMesh
{
public:
	void setSkeletalMeshModel(std::shared_ptr<SkeletalMeshModel> skeletalMeshModel);

	std::shared_ptr<SkeletalMeshModel> skeletalMeshModel_;
	std::vector<std::shared_ptr<Material>> materials;
};

#endif // VENGINE3D_SKELETALMESH_H
