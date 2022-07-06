//
// Created by boris on 7/6/2022.
//
#include "Window.h"
#include <iostream>
//#include <glad/glad.h>
int main() {

	auto w1 = VECore::Window(1280, 720, "w1");
	auto w2 = VECore::Window(1280, 720, "w2");
	//w1.makeContextCurrent();
	gladLoadGL();
	while (!w1.shouldClose() && ! w2.shouldClose()) {
		VECore::Window::poolEvents();
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glClearColor(0.9f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		w1.swapBuffer();
		w2.swapBuffer();
	}
	return 0;
}