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
	virtual ComponentTypeId getComponentTypeId() const override
	{
		return TypeIdManager<IComponent>::getId<T>();
	}
	static ComponentTypeId getTypeId()
	{
		return TypeIdManager<IComponent>::getId<T>();
	}
};

#endif // VENGINE3D_COMPONENT_H
