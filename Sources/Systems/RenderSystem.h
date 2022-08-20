//
// Created by boris on 8/15/2022.
//

#ifndef VENGINE3D_RENDERSYSTEM_H
#define VENGINE3D_RENDERSYSTEM_H
#include "Core/Objects/System.h"
#include "Render/Shader.h"
#include "Render/Texture.h"

class StaticMeshComponent;

class RenderSystem : public System
{
	GENERATE_SYSTEM_BODY()
public:
	RenderSystem(SystemPriority priority = NORMAL_SYSTEM_PRIORITY);
	void resize(int32 width, int32 height);
	virtual void preUpdate(float dt) override;
	virtual void update(float dt) override;
	virtual void postUpdate(float dt) override;
private:
	void updateStaticMeshComponent(StaticMeshComponent* staticMeshComponent, float dt);
	ShaderSource defaultVertexShader;
};

#endif // VENGINE3D_RENDERSYSTEM_H
