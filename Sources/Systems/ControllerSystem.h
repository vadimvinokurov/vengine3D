//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_CONTROLLERSYSTEM_H
#define VENGINE3D_CONTROLLERSYSTEM_H

#include "Core/Objects/System.h"
#include "Input/KeyboardKey.h"
#include "Input/Keystate.h"
#include "Input/MouseKey.h"
class ControllerSystem : public System<ControllerSystem>
{
public:
	ControllerSystem(SystemPriority priority = NORMAL_SYSTEM_PRIORITY);
	void onKeyboardKey(uint32 key, uint32 action);
	void onMouseKey(uint32 key, uint32 action);
	void onMousePosition(float xpos, float ypos);
	virtual void update(float dt) override;

private:
	KeyState keyboardState[512];
	bool keyboardRepeatStatus[512];

	KeyState mouseState[8];
	bool mouseRepeatStatus[8];
};

#endif // VENGINE3D_CONTROLLERSYSTEM_H
