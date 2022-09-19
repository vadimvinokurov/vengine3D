//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_INPUTCOMPONENTS_H
#define VENGINE3D_INPUTCOMPONENTS_H

#include "Core/Objects/Component.h"
#include "Core/Delegate.h"
#include "Input/InputKeyState.h"
#include "Input/InputKey.h"
#include <unordered_map>

class InputComponents : public Component
{
	GENERATE_COMPONENT_BODY(Component)
public:
	InputComponents()
	{
		keysAxis_[VE_KEY_W] = {"MoveForward", 1.0f};
		keysAxis_[VE_KEY_S] = {"MoveForward", -1.0f};
		keysAxis_[VE_KEY_D] = {"MoveRight", 1.0f};
		keysAxis_[VE_KEY_A] = {"MoveRight", -1.0f};

		keysAxis_[VE_MOUSE_X] = {"Turn", 1.0f};
		keysAxis_[VE_MOUSE_Y] = {"LookUp", 1.0f};
	}

	template <class T, class M>
	void bindAxis(const std::string &name, T *objects, M method)
	{
		axisDelegates_[name].connect(objects, method);
	}

	template <class T, class M>
	void bindAction(const std::string &name, InputKeyState keyState, T *objects, M method)
	{
		if (keyState == InputKeyState::PRESSED)
		{
			pressDelegates_[name].connect(objects, method);
		}
		else if (keyState == InputKeyState::RELEASE)
		{
			releaseDelegates_[name].connect(objects, method);
		}
	}

	void inputPressAction(uint32 keyCode)
	{
		auto actionKeyIt = keysAction_.find(keyCode);
		if (actionKeyIt == keysAction_.end())
		{
			return;
		}
		pressDelegates_[actionKeyIt->second].call();
	}

	void inputReleaseAction(uint32 keyCode)
	{
		auto actionKeyIt = keysAction_.find(keyCode);
		if (actionKeyIt == keysAction_.end())
		{
			return;
		}
		releaseDelegates_[actionKeyIt->second].call();
	}

	void inputAxis(uint32 keyCode, float value)
	{
		auto axisKeyIt = keysAxis_.find(keyCode);
		if (axisKeyIt == keysAxis_.end())
		{
			return;
		}
		axisDelegates_[axisKeyIt->second.first].call(value * axisKeyIt->second.second);
	}

private:
	std::unordered_map<std::string, Delegate<float>> axisDelegates_;
	std::unordered_map<std::string, Delegate<>> pressDelegates_;
	std::unordered_map<std::string, Delegate<>> releaseDelegates_;
	std::unordered_map<uint32, std::string> keysAction_;
	std::unordered_map<uint32, std::pair<std::string, float>> keysAxis_;
};

#endif // VENGINE3D_INPUTCOMPONENTS_H
