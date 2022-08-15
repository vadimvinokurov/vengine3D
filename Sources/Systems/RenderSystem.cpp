//
// Created by boris on 8/15/2022.
//

#include "RenderSystem.h"

#include <glad/glad.h>

RenderSystem::RenderSystem(SystemPriority priority) : System<RenderSystem>(priority)
{
	gladLoadGL();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClearDepth(1.0f);

	auto frag = ShaderSource("../Content/Shaders/default.frag", ShaderType::Fragment);
	auto vert = ShaderSource("../Content/Shaders/default.vert", ShaderType::Vertex);
	texture = new Texture("../Content/Texture/test.png");
	defaultShader = new Shader({frag, vert});
}

void RenderSystem::resize(int32 width, int32 height)
{
	glViewport(0, 0, width, height);
}

void RenderSystem::preUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RenderSystem::update(float dt)
{
	// clang-format off
	float vertices[] = {
			1.0f,1.0f,0.0f,
			-1.0f,1.0f,0.0f,
			1.0f,-1.0f,0.0f,
			1.0f,-1.0f,0.0f,
			-1.0f,1.0f,0.0f,
			-1.0f,-1.0f,0.0f

	};
	float textCoord[] = {
			1.0f,0.0f,
			0.0f,0.0f,
			1.0f,1.0f,
			1.0f,1.0f,
			0.0f,0.0f,
			0.0f,1.0f
	};
	// clang-format on

	auto positionAttrib = defaultShader->getAttribute("aPosition");
	auto texCoordAttrib = defaultShader->getAttribute("aTextCoord");
	auto tex0Uniform = defaultShader->getUniform("tex0");

	GLuint verticesBuffer;
	GLuint texCoordBuffer;

	glGenBuffers(1, &verticesBuffer);
	glGenBuffers(1, &texCoordBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textCoord), textCoord, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(positionAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
	glEnableVertexAttribArray(texCoordAttrib);
	glBindVertexArray(0);

	defaultShader->bind();

	texture->bind(tex0Uniform, 0);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	texture->unBind(0);

	defaultShader->unBind();
}

void RenderSystem::postUpdate(float dt)
{
}
