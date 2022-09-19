//
// Created by boris on 19.09.2022.
//

#ifndef VENGINE3D_SKYSPHERE_H
#define VENGINE3D_SKYSPHERE_H

#include "Core/Objects/Entity.h"

class SkySphere : public Entity
{
	GENERATE_ENTITY_BODY(Entity);

public:
	void onCreate() override
	{
		staticMeshComponent = addComponent<StaticMeshComponent>();
		AssetImporter sky = AssetImporter("../Content/Mesh/skysphere/skysphere.fbx");
		auto skySphereMesh = sky.loadMeshes();
		staticMeshComponent->setMaterial(std::make_shared<Material>("../Content/Mesh/skysphere/sky_texture.jpg"));
		staticMeshComponent->setStaticMesh(std::make_shared<StaticMesh>(skySphereMesh[0]));

		staticMeshComponent->transform.scale = Vector3(4000.0f);
	}

	void tick(float dt) override
	{
		staticMeshComponent->transform.rotation *= Quaternion::fromAxisAngle(Vector3(0, 0, 1), 0.0001f);
	}

	void onQuite() override
	{
	}

	StaticMeshComponent *staticMeshComponent;
};

#endif // VENGINE3D_SKYSPHERE_H
