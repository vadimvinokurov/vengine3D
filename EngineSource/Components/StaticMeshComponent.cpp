//
// Created by boris on 8/20/2022.
//

#include "StaticMeshComponent.h"
StaticMeshComponent::StaticMeshComponent()
{
}
void StaticMeshComponent::setStaticMesh(std::shared_ptr<StaticMesh> newStaticMesh)
{
	staticMesh = newStaticMesh;
	needUpdateRenderData = 1;
}
void StaticMeshComponent::setMaterial(std::shared_ptr<Material> newMaterial)
{
	material = newMaterial;
	needUpdateMaterialData = 1;
}
