//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_WORLD_H
#define VENGINE3D_WORLD_H

#include "VEngine.h"

class World : public VEngine
{
public:
	World();
	virtual void onCreate() override;
	virtual void onUpdate(float dt) override;
	virtual void onQuite() override;
};

#endif // VENGINE3D_WORLD_H
