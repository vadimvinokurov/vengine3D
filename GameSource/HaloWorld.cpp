//
// Created by boris on 19.09.2022.
//

#include "HaloWorld.h"
#include "HaloCharacter.h"
#include "AssetImporter.h"

void HaloWorld::onCreate()
{
	World::onCreate();
	auto character = entityManager->createEntity<HaloCharacter>();
}
void HaloWorld::update(float dt)
{
	World::update(dt);
}
void HaloWorld::onQuite()
{
	World::onQuite();
}
