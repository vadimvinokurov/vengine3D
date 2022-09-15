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
#include "Systems/RenderSystem.h"
#include "Systems/ControllerSystem.h"

class Dragon : public Entity
{
	GENERATE_ENTITY_BODY(Entity)
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
	GENERATE_ENTITY_BODY(Entity);
};

World::World()
{
	systemManager = std::make_unique<SystemManager>();
	componentManager = std::make_unique<ComponentManager>();
	entityManager = std::make_unique<EntityManager>(componentManager.get());

	auto controllerSystem = systemManager->addSystem<ControllerSystem>(System::HIGHEST_SYSTEM_PRIORITY);
	VEngine::getWindow()->onKeyboardKeyDelegate.connect(controllerSystem, &ControllerSystem::onInputKey);
	VEngine::getWindow()->onMouseKeyDelegate.connect(controllerSystem, &ControllerSystem::onInputKey);
	VEngine::getWindow()->onCursorPositionDelegate.connect(controllerSystem, &ControllerSystem::onCursorPosition);
	VEngine::getWindow()->onCursorDeltaDelegate.connect(controllerSystem, &ControllerSystem::onCursorDelta);

	auto renderSystem = systemManager->addSystem<RenderSystem>(System::LOWEST_SYSTEM_PRIORITY);
	VEngine::getWindow()->onWindowResizeDelegate.connect(renderSystem, &RenderSystem::onWindowResize);
	VEngine::getWindow()->onWindowResizeDelegate.connect(controllerSystem, &ControllerSystem::onWindowResize);

	auto [width, height] = VEngine::getWindow()->getSize();

	renderSystem->onWindowResize(width, height);
	controllerSystem->onWindowResize(width, height);
	onCreate();
}

SkyBox *skyBox;
void World::onCreate()
{
	Dragon *dragon = entityManager->createEntity<Dragon>();
	auto inputComponents = dragon->addComponent<InputComponents>();
	inputComponents->bindAxis("Turn", dragon, &Dragon::turn);
	inputComponents->bindAxis("LookUp", dragon, &Dragon::lookUp);

	auto cameraComponent = dragon->addComponent<CameraComponent>();
	inputComponents->bindAxis("MoveForward", cameraComponent, &CameraComponent::moveAlongDirection);
	inputComponents->bindAxis("MoveRight", cameraComponent, &CameraComponent::moveAlongSide);

	skyBox = entityManager->createEntity<SkyBox>();

	// AssetImporter fbx = AssetImporter("../Content/Mesh/astroboy/astroBoy_walk_Max.dae");
	AssetImporter fbx = AssetImporter("../Content/Mesh/halo/source/halo.fbx");
	AssetImporter sky = AssetImporter("../Content/Mesh/skysphere/skysphere.fbx");
	auto skySphere = sky.loadMeshes();

	StaticMeshComponent *staticMeshComponent2 = skyBox->addComponent<StaticMeshComponent>();
	staticMeshComponent2->setStaticMesh(std::make_shared<StaticMesh>(skySphere[0]));
	staticMeshComponent2->setMaterial(std::make_shared<Material>("../Content/Mesh/skysphere/sky_texture.jpg"));
	staticMeshComponent2->transform.scale = Vector3(4000.0f);

	SkeletalMeshComponent *skeletalMeshComponent = dragon->addComponent<SkeletalMeshComponent>();

	auto skeletalMeshModel = std::make_shared<SkeletalMeshModel>(fbx.getSkeletalMesh());
	skeletalMeshComponent->skeletalMesh.setSkeletalMeshModel(skeletalMeshModel);

	std::vector<ShaderSource> shaderSource;
	shaderSource.emplace_back("../Content/Shaders/default.frag", ShaderType::Fragment);
	shaderSource.emplace_back("../Content/Shaders/animdefault.vert", ShaderType::Vertex);

	//	skeletalMeshComponent->skeletalMesh.materials[0] =
	//		std::make_shared<Material>("../Content/Mesh/astroboy/boy_10.JPG", shaderSource);
	//	skeletalMeshComponent->skeletalMesh.materials[1] =
	//		std::make_shared<Material>("../Content/Mesh/astroboy/boy_10.JPG", shaderSource);
	//	skeletalMeshComponent->skeletalMesh.materials[2] =
	//		std::make_shared<Material>("../Content/Mesh/astroboy/boy_10.JPG", shaderSource);
	//	skeletalMeshComponent->skeletalMesh.materials[3] =
	//		std::make_shared<Material>("../Content/Mesh/astroboy/boy_10.JPG", shaderSource);
	skeletalMeshComponent->skeletalMesh.materials[0] =
		std::make_shared<Material>("../Content/Mesh/halo/textures/ODST_Shoulder_Mat_BaseColor.png", shaderSource);
	skeletalMeshComponent->skeletalMesh.materials[1] =
		std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Chest_Mat_BaseColor.png", shaderSource);
	skeletalMeshComponent->skeletalMesh.materials[2] =
		std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Arms_Mat_BaseColor.png", shaderSource);
	skeletalMeshComponent->skeletalMesh.materials[3] =
		std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Legs_Mat_BaseColor.png", shaderSource);
	skeletalMeshComponent->skeletalMesh.materials[4] =
		std::make_shared<Material>("../Content/Mesh/halo/textures/ODST_Shoulder_Mat_BaseColor.png", shaderSource);
	skeletalMeshComponent->skeletalMesh.materials[5] =
		std::make_shared<Material>("../Content/Mesh/halo/textures/ODST_Shoulder_Mat_BaseColor.png", shaderSource);
	skeletalMeshComponent->skeletalMesh.materials[6] =
		std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Ears_Mat_BaseColor.png", shaderSource);
	skeletalMeshComponent->skeletalMesh.materials[7] =
		std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Helmet_Mat_BaseColor.png", shaderSource);
	skeletalMeshComponent->skeletalMesh.materials[8] =
		std::make_shared<Material>("../Content/Mesh/halo/textures/Spartan_Ears_Mat_BaseColor.png", shaderSource);

	Skeleton restPose = fbx.getSkeleton();
	auto animations = fbx.getAnimations();

	skeletalMeshComponent->skeleton = std::make_shared<Skeleton>(restPose);

	skeletalMeshComponent->animation = std::make_shared<Animation>(animations.begin()->second);
	skeletalMeshComponent->animation->setLooping(true);
}
void World::update(float dt)
{
	auto sm = skyBox->getComponent<StaticMeshComponent>();
	sm->transform.rotation *= Quaternion::fromAxisAngle(Vector3(0, 0, 1), 0.0001f);

	systemManager->update(dt);
	entityManager->removeDestroyedEntities();
}
void World::onQuite()
{
}
