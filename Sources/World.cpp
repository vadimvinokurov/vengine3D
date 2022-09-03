//
// Created by boris on 8/16/2022.
//

#include "World.h"
#include "Core/Objects/Entity.h"
#include "Components/InputComponents.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CameraComponent.h"
#include "AssetImporter.h"
#include "SkeletalMesh.h"

class Dragon : public Entity
{
	GENERATE_ENTITY_BODY()
public:
	void lookUp(float amount)
	{
		if (amount == 0.0f)
		{
			return;
		}
		getComponent<CameraComponent>()->addPitchInput(amount);
	}

	void turn(float amount)
	{
		if (amount == 0.0f)
		{
			return;
		}
		getComponent<CameraComponent>()->addYawInput(amount);
	}
};

class SkyBox : public Entity
{
	GENERATE_ENTITY_BODY();
};

World::World()
{
}
SkyBox *skyBox;
void World::onCreate()
{
	VEngine::onCreate();

	Dragon *dragon = entityManager->createEntity<Dragon>();
	InputComponents *inputComponents = dragon->addComponent<InputComponents>();
	inputComponents->bindAxis("Turn", dragon, &Dragon::turn);
	inputComponents->bindAxis("LookUp", dragon, &Dragon::lookUp);

	CameraComponent *cameraComponent = dragon->addComponent<CameraComponent>();
	inputComponents->bindAxis("MoveForward", cameraComponent, &CameraComponent::moveAlongDirection);
	inputComponents->bindAxis("MoveRight", cameraComponent, &CameraComponent::moveAlongSide);

	skyBox = entityManager->createEntity<SkyBox>();

	AssetImporter fbx = AssetImporter("E:\\Work\\vengine3D\\Content\\Mesh\\woman\\Woman.gltf");
	auto staticMeshs = fbx.loadMeshes();
	AssetImporter sky = AssetImporter("E:\\Work\\vengine3D\\Content\\Mesh\\skysphere\\skysphere.fbx");
	auto skySphere = sky.loadMeshes();

	StaticMeshComponent *staticMeshComponent2 = skyBox->addComponent<StaticMeshComponent>();
	staticMeshComponent2->setStaticMesh(std::make_shared<StaticMesh>(skySphere[0]));
	staticMeshComponent2->setMaterial(
		std::make_shared<Material>("E:\\Work\\vengine3D\\Content\\Mesh\\skysphere\\sky_texture.jpg"));
	staticMeshComponent2->transform.scale = Vector3(4000.0f);

	SkeletalMeshComponent *skeletalMeshComponent = dragon->addComponent<SkeletalMeshComponent>();

	auto skeletalMeshModel = std::make_shared<SkeletalMeshModel>(fbx.getSkeletalMesh());
	skeletalMeshComponent->skeletalMesh.setSkeletalMeshModel(skeletalMeshModel);

	spdlog::warn("size - {}", skeletalMeshComponent->skeletalMesh.materials.size());
	skeletalMeshComponent->skeletalMesh.materials[0] =
		std::make_shared<Material>("E:\\Work\\vengine3D\\Content\\Mesh\\woman\\Woman.png");
	//	skeletalMeshComponent->skeletalMesh.materials[1] = std::make_shared<Material>(
	//		"E:\\Work\\vengine3D\\Content\\Mesh\\halo\\textures\\Spartan_Chest_Mat_BaseColor.png");
	//	skeletalMeshComponent->skeletalMesh.materials[2] = std::make_shared<Material>(
	//		"E:\\Work\\vengine3D\\Content\\Mesh\\halo\\textures\\Spartan_Arms_Mat_BaseColor.png");
	//	skeletalMeshComponent->skeletalMesh.materials[3] = std::make_shared<Material>(
	//		"E:\\Work\\vengine3D\\Content\\Mesh\\halo\\textures\\Spartan_Legs_Mat_BaseColor.png");
	//	skeletalMeshComponent->skeletalMesh.materials[4] = std::make_shared<Material>(
	//		"E:\\Work\\vengine3D\\Content\\Mesh\\halo\\textures\\ODST_Shoulder_Mat_BaseColor.png");
	//	skeletalMeshComponent->skeletalMesh.materials[5] = std::make_shared<Material>(
	//		"E:\\Work\\vengine3D\\Content\\Mesh\\halo\\textures\\ODST_Shoulder_Mat_BaseColor.png");
	//	skeletalMeshComponent->skeletalMesh.materials[6] = std::make_shared<Material>(
	//		"E:\\Work\\vengine3D\\Content\\Mesh\\halo\\textures\\Spartan_Ears_Mat_BaseColor.png");
	//	skeletalMeshComponent->skeletalMesh.materials[7] = std::make_shared<Material>(
	//		"E:\\Work\\vengine3D\\Content\\Mesh\\halo\\textures\\Spartan_Helmet_Mat_BaseColor.png");
	//	skeletalMeshComponent->skeletalMesh.materials[8] = std::make_shared<Material>(
	//		"E:\\Work\\vengine3D\\Content\\Mesh\\halo\\textures\\Spartan_Ears_Mat_BaseColor.png");

	Skeleton bindPose = fbx.getSkeleton();
	auto animations = fbx.getAnimations();

	for (const auto &anim : animations)
	{
		spdlog::warn("Anim: {}", anim.first);
	}
}
void World::onUpdate(float dt)
{
	VEngine::onUpdate(dt);
	auto sm = skyBox->getComponent<StaticMeshComponent>();
	sm->transform.rotation *= Quaternion::fromAxisAngle(Vector3(0, 0, 1), 0.0001f);
}
void World::onQuite()
{
	VEngine::onQuite();
}
