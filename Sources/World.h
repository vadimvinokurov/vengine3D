//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_WORLD_H
#define VENGINE3D_WORLD_H

#include "Core/VEngine.h"

class World : public VEngine
{
public:
	virtual void onCreate() override;
	virtual void onUpdate(float dt) override;
	virtual void onQuite() override;

private:
	World();

	friend World *getWorld();
};

inline World *getWorld()
{
	static std::unique_ptr<World> world(new World());
	return world.get();
}

#endif // VENGINE3D_WORLD_H
