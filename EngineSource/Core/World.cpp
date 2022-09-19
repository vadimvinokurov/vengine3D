//
// Created by boris on 8/16/2022.
//

#include "World.h"
#include "Core/VEngine.h"
#include "Core/Objects/Entity.h"
#include "Components/InputComponents.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CameraComponent.h"
#include "AssetImporter.h"
#include "SkeletalMesh.h"
#include "Systems/RenderSystem.h"
#include "Systems/ControllerSystem.h"
#include "Entities/SkySphere.h"

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
}

void World::onCreate()
{
	skySphere = entityManager->createEntity<SkySphere>();
}
void World::update(float dt)
{
	skySphere->tick(dt);

	systemManager->update(dt);
	entityManager->removeDestroyedEntities();
}
void World::onQuite()
{
}
