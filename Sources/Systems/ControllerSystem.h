//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_CONTROLLERSYSTEM_H
#define VENGINE3D_CONTROLLERSYSTEM_H

#include "Core/Objects/System.h"
#include "Input/KeyboardKey.h"
#include "Input/Keystate.h"
#include "Input/MouseKey.h"
#include <array>
class ControllerSystem : public System
{
	GENERATE_SYSTEM_BODY()
public:
	ControllerSystem(SystemPriority priority = NORMAL_SYSTEM_PRIORITY);
	void onKeyboardKey(uint32 key, uint32 action);
	void onMouseKey(uint32 key, uint32 action);
	void onMousePosition(float xpos, float ypos);
	virtual void update(float dt) override;

private:
	std::array<KeyState, 512> keyboardState;
	std::array<bool, 512> keyboardRepeatStatus;

	std::array<KeyState, 8>  mouseState;
	std::array<bool, 8> mouseRepeatStatus;
};

#endif // VENGINE3D_CONTROLLERSYSTEM_H
