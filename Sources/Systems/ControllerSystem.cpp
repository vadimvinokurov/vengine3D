//
// Created by boris on 8/16/2022.
//

#include "ControllerSystem.h"
#include "World.h"
#include "Components/InputComponents.h"

ControllerSystem::ControllerSystem(SystemPriority priority) : System<ControllerSystem>(priority)
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
	auto[begin, end] = getWorld()->getComponents<InputComponents>();

	for(auto it = begin; it != end; ++it){
		it->press();
	}
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
