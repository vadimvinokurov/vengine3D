//
// Created by boris on 19.09.2022.
//

#include "AstroWorld.h"

//
// Created by boris on 19.09.2022.
//

#include "AstroWorld.h"
#include "AstroCharacter.h"

void AstroWorld::onCreate()
{
	World::onCreate();
	auto character = entityManager->createEntity<AstroCharacter>();
}
void AstroWorld::update(float dt)
{
	World::update(dt);
}
void AstroWorld::onQuite()
{
	World::onQuite();
}
