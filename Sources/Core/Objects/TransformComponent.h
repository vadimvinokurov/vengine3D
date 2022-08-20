//
// Created by boris on 8/20/2022.
//

#ifndef VENGINE3D_TRANSFORMCOMPONENT_H
#define VENGINE3D_TRANSFORMCOMPONENT_H

#include "Component.h"
#include "Transform.h"
class TransformComponent : public Component
{
protected:
	Transform transform;
};

#endif // VENGINE3D_TRANSFORMCOMPONENT_H
