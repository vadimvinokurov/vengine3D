//
// Created by boris on 7/7/2022.
//

#ifndef VENGINE3D_RENDERENGINE_H
#define VENGINE3D_RENDERENGINE_H

#include "EngineCore.h"
#include "Shader.h"

class RenderEngine {
public:
	RenderEngine();
	void clear();
	void resize(int32 width, int32 height);
	void update();
	Shader* defaultShader;
};


#endif	//VENGINE3D_RENDERENGINE_H
