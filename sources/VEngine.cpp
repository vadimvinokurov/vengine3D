//
// Created by boris on 7/6/2022.
//

#include "VEngine.h"
#include "Window.h"
#include <glad/glad.h>
void VEngine::run() {
	auto w1 = Window(1280, 720, "w1");
	gladLoadGL();
	w1.OnWindowResizeDelegate.connect(this, &VEngine::resize);
	while (!w1.shouldClose()) {
		Window::poolEvents();
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glClearColor(0.9f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		w1.swapBuffer();
	}
}
void VEngine::resize(int32 w, int32 h) {
	spdlog::warn("Windows has been resize {:08d} {:08d}", w, h);
}
