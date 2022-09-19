//
// Created by boris on 8/20/2022.
//

#ifndef VENGINE3D_OPENGL_GLAD_H
#define VENGINE3D_OPENGL_GLAD_H

#include <glad/glad.h>

inline void initOpenGl()
{
	gladLoadGL();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClearDepth(1.0f);
}

static_assert(sizeof(GLuint) == sizeof(uint32));

#endif // VENGINE3D_OPENGL_GLAD_H
