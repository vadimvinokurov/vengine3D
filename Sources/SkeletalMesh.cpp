//
// Created by boris on 8/23/2022.
//

#include "SkeletalMesh.h"

void SkeletalMesh::setSkeletalMeshModel(std::shared_ptr<SkeletalMeshModel> skeletalMeshModel)
{
	skeletalMeshModel_ = skeletalMeshModel;
	materials.resize(skeletalMeshModel_->getSectionCount());
}
