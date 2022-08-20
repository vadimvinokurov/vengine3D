//
// Created by boris on 7/6/2022.
//

#ifndef VENGINE3D_WINDOW_H
#define VENGINE3D_WINDOW_H

#include "EngineCore.h"
#include "Delegate.h"

struct GLFWwindow;

class Window
{
public:
	Window(int32 width = 1280, int32 height = 720, const char *title = "VEngine");
	void makeContextCurrent();
	std::pair<int32, int32> getSize() const;
	float getRation() const;
	bool shouldClose() const;
	void swapBuffer();
	void setTitle(const std::string &title);
	static void poolEvents();


	Delegate<int32, int32> onWindowResizeDelegate;
	Delegate<int32, int32> onKeyboardKeyDelegate;
	Delegate<int32, int32> onMouseKeyDelegate;
	Delegate<float, float> onMousePositionDelegate;
	Delegate<float, float> onMouseDeltaPositionDelegate;

	~Window();

private:
	void glfwCallbackInitialization();
	GLFWwindow *window = nullptr;

	bool mouseLockFlag = false;
	double lockXpos;
	double lockYpos;
};

#endif // VENGINE3D_WINDOW_H
