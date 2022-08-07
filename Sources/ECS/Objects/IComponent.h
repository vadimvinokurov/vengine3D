//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_ICOMPONENT_H
#define VENGINE3D_ICOMPONENT_H

#include "VObject.h"
#include "ECS/Utils/IdManagers.h"
#include "ECS/Types.h"
#include "IEntity.h"

class IComponent : public VObject
{
	friend class ComponentManage;
public:
	IComponent() : owner_(INVALID_ID), active_(true)
	{
	}
	virtual ~IComponent() = default;

	ComponentId getComponentId() const
	{
		return id_;
	}

	EntityId getOwner() const
	{
		return owner_;
	}

	inline void SetActive(bool active) {
		active_ = active;
	}
	inline bool IsActive() const {
		return active_;
	}

	virtual ComponentTypeId getComponentTypeId() const = 0;

protected:
	ComponentId hash_;
	ComponentId id_;
	EntityId owner_;
	bool active_;
};

#endif // VENGINE3D_ICOMPONENT_H
