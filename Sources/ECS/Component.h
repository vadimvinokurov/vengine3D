//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_COMPONENT_H
#define VENGINE3D_COMPONENT_H

#include "IComponent.h"

template <typename T>
class Component : public IComponent
{
public:
	const ComponentTypeId getComponentTypeId() const override
	{
		return TypeIdManager::getId<T>();
	}
};

#endif // VENGINE3D_COMPONENT_H
