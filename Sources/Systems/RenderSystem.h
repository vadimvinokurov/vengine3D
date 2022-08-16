//
// Created by boris on 8/15/2022.
//

#ifndef VENGINE3D_RENDERSYSTEM_H
#define VENGINE3D_RENDERSYSTEM_H
#include "Core/Objects/System.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

class RenderSystem : public System
{
	GENERATE_SYSTEM_BODY()
public:
	RenderSystem(SystemPriority priority = NORMAL_SYSTEM_PRIORITY);
	void resize(int32 width, int32 height);
	virtual void preUpdate(float dt) override;
	virtual void update(float dt) override;
	virtual void postUpdate(float dt) override;
	Shader *defaultShader;
	Texture *texture;
};

#endif // VENGINE3D_RENDERSYSTEM_H
