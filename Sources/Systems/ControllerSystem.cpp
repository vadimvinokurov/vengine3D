//
// Created by boris on 8/16/2022.
//

#include "ControllerSystem.h"
#include "World.h"
#include "Components/InputComponents.h"

ControllerSystem::ControllerSystem(SystemPriority priority) : System(priority)
{
	for (KeyState &key : keyboardState)
	{
		key = KeyState::FREE;
	}
	for (bool &r : keyboardRepeatStatus)
	{
		r = false;
	}
	for (KeyState &key : mouseState)
	{
		key = KeyState::FREE;
	}
	for (bool &r : mouseRepeatStatus)
	{
		r = false;
	}
}
void ControllerSystem::update(float dt)
{
	auto [inputComponentIt, end] = getWorld()->getComponents<InputComponents>();
	if (inputComponentIt == end)
	{
		return;
	}

	for (uint32 key = 0; key < keyboardState.size(); ++key)
	{
		if (keyboardState[key] != KeyState::FREE)
		{
			inputComponentIt->input(key, keyboardState[key]);
			keyboardState[key] = KeyState::FREE;
		}
	}
	for (uint32 key = 0; key < mouseState.size(); ++key)
	{
		if (mouseState[key] != KeyState::FREE)
		{
			inputComponentIt->input(key, mouseState[key]);
			mouseState[key] = KeyState::FREE;
		}
	}

	for (uint32 key = 0; key < keyboardRepeatStatus.size(); ++key)
	{
		if (keyboardRepeatStatus[key])
		{
			inputComponentIt->input(key, KeyState::REPEATE);
		}
	}
	for (uint32 key = 0; key < mouseRepeatStatus.size(); ++key)
	{
		if (mouseRepeatStatus[key])
		{
			inputComponentIt->input(key, KeyState::REPEATE);
		}
	}

	inputComponentIt->inputMouse(deltaxpos, deltaypos);
	deltaxpos = deltaypos = 0.0f;
}

void ControllerSystem::onKeyboardKey(uint32 key, uint32 action)
{
	KeyState state = static_cast<KeyState>(action);
	if (state == KeyState::PRESSED)
	{
		keyboardState[key % 512] = KeyState::PRESSED;
		keyboardRepeatStatus[key % 512] = true;
	}
	else if (state == KeyState::RELEASE)
	{
		keyboardState[key % 512] = KeyState::RELEASE;
		keyboardRepeatStatus[key % 512] = false;
	}
}
void ControllerSystem::onMouseKey(uint32 key, uint32 action)
{
	KeyState state = static_cast<KeyState>(action);
	if (state == KeyState::PRESSED)
	{
		mouseState[key % 8] = KeyState::PRESSED;
		mouseRepeatStatus[key % 8] = true;
	}
	else if (state == KeyState::RELEASE)
	{
		mouseState[key % 8] = KeyState::RELEASE;
		mouseRepeatStatus[key % 8] = false;
	}
}
void ControllerSystem::onMousePosition(float xpos, float ypos)
{
}
void ControllerSystem::onWindowResize(int32 width, int32 height)
{
}
void ControllerSystem::onMouseDeltaPosition(float dxpos, float dypos)
{
	deltaxpos = dxpos / mouseScale;
	deltaypos = dypos / mouseScale;
}
