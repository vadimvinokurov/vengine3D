//
// Created by boris on 8/16/2022.
//

#ifndef VENGINE3D_INPUTCOMPONENTS_H
#define VENGINE3D_INPUTCOMPONENTS_H

#include "Core/Objects/Component.h"
#include "Core/Delegate.h"
#include "Input/Keystate.h"
#include "Input/KeyboardKey.h"
#include "Input/MouseKey.h"
#include <unordered_map>

class InputComponents : public Component
{
	GENERATE_COMPONENT_BODY()
public:
	InputComponents()
	{
		keysAxis_[VE_KEY_W] = {"MoveForward", 1.0f};
		keysAxis_[VE_KEY_S] = {"MoveForward", -1.0f};
		keysAxis_[VE_KEY_D] = {"MoveRight", 1.0f};
		keysAxis_[VE_KEY_A] = {"MoveRight", -1.0f};

		keysAxis_[VE_MOUSE_X] = {"Turn", 1.0f};
		keysAxis_[VE_MOUSE_Y] = {"LookUp", 1.0f};

		keysAction_[VE_MOUSE_BUTTON_RIGHT] = "CameraLock";
	}

	template <class T, class M>
	void bindAxis(const std::string &name, T *objects, M method)
	{
		axisDelegates_[name].connect(objects, method);
	}

	template <class T, class M>
	void bindAction(const std::string &name, KeyState keyState, T *objects, M method)
	{
		if (keyState == KeyState::PRESSED)
		{
			pressDelegates_[name].connect(objects, method);
		}
		else if (keyState == KeyState::RELEASE)
		{
			releaseDelegates_[name].connect(objects, method);
		}
	}

	void input(uint32 keyCode, KeyState keyState)
	{
		auto actionIt = keysAction_.find(keyCode);
		if(actionIt != keysAction_.end()){
			if(keyState == KeyState::PRESSED) {
				pressDelegates_[actionIt->second].call();
				return;
			} else if (keyState == KeyState::RELEASE) {
				releaseDelegates_[actionIt->second].call();
				return;
			}
		}

		auto axisIt = keysAxis_.find(keyCode);
		if(axisIt != keysAxis_.end() && keyState == KeyState::REPEATE){
			axisDelegates_[axisIt->second.first].call(axisIt->second.second);
			return;
		}
	}

	void inputMouse(float xmouse, float ymouse)
	{
		auto axisXIt = keysAxis_.find(VE_MOUSE_X);
		if(axisXIt != keysAxis_.end()){
			axisDelegates_[axisXIt->second.first].call(xmouse * axisXIt->second.second);
		}
		auto axisYIt = keysAxis_.find(VE_MOUSE_Y);
		if(axisYIt != keysAxis_.end()){
			axisDelegates_[axisYIt->second.first].call(ymouse * axisYIt->second.second);
		}
	}

private:
	std::unordered_map<std::string, Delegate<float>> axisDelegates_;
	std::unordered_map<std::string, Delegate<>> pressDelegates_;
	std::unordered_map<std::string, Delegate<>> releaseDelegates_;
	std::unordered_map<uint32, std::string> keysAction_;
	std::unordered_map<uint32, std::pair<std::string, float>> keysAxis_;
};

#endif // VENGINE3D_INPUTCOMPONENTS_H
