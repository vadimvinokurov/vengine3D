//
// Created by boris on 7/7/2022.
//

#include "RenderEngine.h"
#include <glad/glad.h>

RenderEngine::RenderEngine() {
	gladLoadGL();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClearDepth(1.0f);

	auto frag = ShaderSource("E:/Work/vengine3D/Content/Shaders/default.frag", ShaderType::Fragment);
	auto vert = ShaderSource("E:/Work/vengine3D/Content/Shaders/default.vert", ShaderType::Vertex);

	defaultShader = new Shader({frag, vert});
}

void RenderEngine::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RenderEngine::resize(int32 width, int32 height) {
	glViewport(0, 0, width, height);
}

void RenderEngine::update() {}
