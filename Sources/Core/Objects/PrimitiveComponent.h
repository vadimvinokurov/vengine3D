//
// Created by boris on 8/20/2022.
//

#ifndef VENGINE3D_PRIMITIVECOMPONENT_H
#define VENGINE3D_PRIMITIVECOMPONENT_H

#include "TransformComponent.h"
#include "Rendering/RenderObjectData.h"
class PrimitiveComponent : public TransformComponent
{
public:
	PrimitiveComponent(){
		needUpdateRenderData = 1;
		needUpdateMaterialData = 1;
	}

protected:
	RenderableObjectData renderData;
	uint8 needUpdateRenderData : 1;
	uint8 needUpdateMaterialData : 1;
};

#endif // VENGINE3D_PRIMITIVECOMPONENT_H
