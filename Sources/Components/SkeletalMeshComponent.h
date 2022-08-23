//
// Created by boris on 8/23/2022.
//

#ifndef VENGINE3D_SKELETALMESHCOMPONENT_H
#define VENGINE3D_SKELETALMESHCOMPONENT_H

#include "Core/Objects/PrimitiveComponent.h"
#include "SkeletalMesh.h"

class SkeletalMeshComponent : public PrimitiveComponent
{
	friend class RenderSystem;
	GENERATE_COMPONENT_BODY()
public:
	SkeletalMesh skeletalMesh;
};

#endif // VENGINE3D_SKELETALMESHCOMPONENT_H
