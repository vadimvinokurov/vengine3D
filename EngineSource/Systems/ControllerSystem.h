//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_CONTROLLERSYSTEM_H
#define VENGINE3D_CONTROLLERSYSTEM_H

#include "Core/Objects/System.h"
#include "Input/InputKey.h"
#include "Input/InputKeyState.h"
class ControllerSystem : public System
{
	GENERATE_SYSTEM_BODY()
public:
	ControllerSystem(SystemPriority priority = NORMAL_SYSTEM_PRIORITY);
	void onInputKey(uint32 key, uint32 action);
	void onWindowResize(int32 width, int32 height);
	void onCursorPosition(float xpos, float ypos);
	void onCursorDelta(float dxpos, float dypos);

	virtual void update(float dt) override;
private:
	std::vector<InputKey> pressedKey;
	std::vector<InputKey> releasedKey;
	std::vector<InputKey> repeatingKey;
	float cursorDeltaXPos = 0.0f;
	float cursorDeltaYPos = 0.0f;

	static constexpr float cursorDeltaScale = 1 / 100.0f;
};

#endif // VENGINE3D_CONTROLLERSYSTEM_H
