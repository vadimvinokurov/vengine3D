//
// Created by boris on 8/9/2022.
//

#ifndef VENGINE3D_SYSTEM_H
#define VENGINE3D_SYSTEM_H

#include "ISystem.h"

template <typename T>
class System : public ISystem
{
public:
	System(SystemPriority priority = NORMAL_SYSTEM_PRIORITY, float updateInterval = -1.0f)
		: ISystem(priority, updateInterval){};
	virtual SystemTypeId getSystemTypeId() const override
	{
		return TypeIdManager<ISystem>::getId<T>();
	}
	static SystemTypeId getTypeId()
	{
		return TypeIdManager<ISystem>::getId<T>();
	}
};

#endif // VENGINE3D_SYSTEM_H
