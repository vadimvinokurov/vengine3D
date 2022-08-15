//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_ICOMPONENT_H
#define VENGINE3D_ICOMPONENT_H

#include "VObject.h"
#include "Core/Utils/IdManagers.h"

class IComponent : public VObject
{
	friend class ComponentManager;

public:
	IComponent() = default;
	virtual ~IComponent() = default;

	ComponentId getComponentId() const
	{
		return id_;
	}

	EntityId getOwner() const
	{
		return owner_;
	}

	inline void SetActive(bool active)
	{
		active_ = active;
	}
	inline bool IsActive() const
	{
		return active_;
	}

	virtual ComponentTypeId getComponentTypeId() const = 0;

protected:
	ComponentId hash_ = {};
	ComponentId id_ = INVALID_ID;
	EntityId owner_ = INVALID_ID;
	bool active_ = true;
};

#endif // VENGINE3D_ICOMPONENT_H
