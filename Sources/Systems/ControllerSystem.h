//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_CONTROLLERSYSTEM_H
#define VENGINE3D_CONTROLLERSYSTEM_H

#include "ECS/ECS.h"
class ControllerSystem : public System<ControllerSystem>
{
public:
	ControllerSystem(SystemPriority priority = NORMAL_SYSTEM_PRIORITY);
	void onKeyboardKey(uint32 key, uint32 action);
	void onMouseKey(uint32 key, uint32 action);
	void onMousePosition(float xpos, float ypos);
	virtual void preUpdate(float dt) override;
	virtual void update(float dt) override;
	virtual void postUpdate(float dt) override;
};

#endif // VENGINE3D_CONTROLLERSYSTEM_H
