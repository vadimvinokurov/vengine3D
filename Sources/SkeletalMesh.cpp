//
// Created by boris on 8/23/2022.
//

#include "SkeletalMesh.h"

void SkeletalMesh::addSkeletalMeshElement(const std::shared_ptr<SkeletalMeshElement> &skeletalMeshElement)
{
	skeletalMeshElements.push_back(skeletalMeshElement);
	materials.resize(skeletalMeshElements.size());
}
