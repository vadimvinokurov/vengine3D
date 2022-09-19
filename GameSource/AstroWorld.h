//
// Created by boris on 19.09.2022.
//

#ifndef VENGINE3D_ASTROWORLD_H
#define VENGINE3D_ASTROWORLD_H

#include "Core/World.h"

class AstroWorld : public World
{
public:
	void onCreate() override;
	void update(float dt) override;
	void onQuite() override;
};


#endif // VENGINE3D_ASTROWORLD_H
