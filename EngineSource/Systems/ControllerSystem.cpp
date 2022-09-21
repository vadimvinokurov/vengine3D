//
// Created by boris on 8/16/2022.
//

#include "ControllerSystem.h"
#include "Core/World.h"
#include "Core/VEngine.h"
#include "Components/InputComponents.h"

ControllerSystem::ControllerSystem(SystemPriority priority) : System(priority)
{
}

void ControllerSystem::update(float dt)
{
	auto [inputComponentIt, end] = VEngine::getWorld()->getComponents<InputComponents>();
	if (inputComponentIt == end)
	{
		return;
	}

	for (auto &key : pressedKey)
	{
		(*inputComponentIt)->inputPressAction(key);
	}

	for (auto &key : releasedKey)
	{
		(*inputComponentIt)->inputReleaseAction(key);
	}

	pressedKey.clear();
	releasedKey.clear();

	for (auto &key : repeatingKey)
	{
		(*inputComponentIt)->inputAxis(key, 1);
	}

	(*inputComponentIt)->inputAxis(VE_MOUSE_X, cursorDeltaXPos * cursorDeltaScale);
	(*inputComponentIt)->inputAxis(VE_MOUSE_Y, cursorDeltaYPos * cursorDeltaScale);
	cursorDeltaXPos = cursorDeltaYPos = 0.0f;
}

void ControllerSystem::onInputKey(uint32 key, uint32 action)
{
	InputKeyState state = static_cast<InputKeyState>(action);
	if (state == InputKeyState::PRESSED)
	{
		pressedKey.push_back(key);
		repeatingKey.push_back(key);
	}
	else if (state == InputKeyState::RELEASE)
	{
		releasedKey.push_back(key);
		auto it = std::find(repeatingKey.begin(), repeatingKey.end(), key);
		if (it != repeatingKey.end() && !repeatingKey.empty())
		{
			std::swap(*it, repeatingKey.back());
			repeatingKey.pop_back();
		}
	}
}

void ControllerSystem::onCursorPosition(float xpos, float ypos)
{
}
void ControllerSystem::onWindowResize(int32 width, int32 height)
{
}
void ControllerSystem::onCursorDelta(float dxpos, float dypos)
{
	cursorDeltaXPos = dxpos;
	cursorDeltaYPos = dypos;
}
