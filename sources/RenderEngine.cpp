//
// Created by boris on 7/7/2022.
//

#include "RenderEngine.h"
#include <glad/glad.h>

RenderEngine::RenderEngine() {
	gladLoadGL();
}

void RenderEngine::clear() {
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RenderEngine::resize(int32 width, int32 height) {
	spdlog::warn("Windows has been resize {:08d} {:08d}", width, height);
}
