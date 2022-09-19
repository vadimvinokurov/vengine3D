//
// Created by boris on 8/20/2022.
//

#ifndef VENGINE3D_STATICMESHCOMPONENT_H
#define VENGINE3D_STATICMESHCOMPONENT_H

#include "Core/Objects/PrimitiveComponent.h"
#include "StaticMesh.h"
#include "Material.h"


class StaticMeshComponent : public PrimitiveComponent
{
	friend class RenderSystem;
	GENERATE_COMPONENT_BODY(PrimitiveComponent)
public:
	StaticMeshComponent();
	void setStaticMesh(std::shared_ptr<StaticMesh> newStaticMesh);
	void setMaterial(std::shared_ptr<Material> newMaterial);
protected:
	std::shared_ptr<StaticMesh> staticMesh;
	std::shared_ptr<Material> material;
};

#endif // VENGINE3D_STATICMESHCOMPONENT_H
