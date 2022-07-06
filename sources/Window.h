//
// Created by boris on 7/6/2022.
//

#ifndef VENGINE3D_WINDOW_H
#define VENGINE3D_WINDOW_H

#include "EngineCore.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace VECore {
	class Window {
	public:
		Window(int32 width = 1280, int32 height = 720, const char* title = "VEngine");
		void makeContextCurrent();
		std::pair<int32, int32> getSize() const;
		float32 getRation() const;
		bool shouldClose() const;
		void swapBuffer();
		static void poolEvents();
		~Window();

	private:
		GLFWwindow* window = nullptr;
	};
}



#endif	//VENGINE3D_WINDOW_H
