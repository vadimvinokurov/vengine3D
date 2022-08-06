//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_IENTITY_H
#define VENGINE3D_IENTITY_H

#include "Handle.h"
#include "IdManager.h"

using EntityId = ECS::Handle64;
using EntityTypeId = TypeId;

class IEntity
{
public:
	IEntity() : active_(true){};
	virtual ~IEntity() = default;

	EntityId getEntityId() const
	{
		return id_;
	}

	template <typename T, typename... Args>
	void addComponent(Args &&...args)
	{
	}

	template <typename T>
	void getComponent()
	{
	}
	void removeComponent()
	{
	}

	void setActive(bool active)
	{
		if (active_ == active)
			return;

		if (active == false)
		{
			this->OnDisable();
		}
		else
		{
			this->OnEnable();
		}

		active_ = active;
	}

	bool isActive()
	{
		return active_;
	}

	bool operator==(const IEntity &other) const
	{
		return id_ == other.id_;
	}
	bool operator!=(const IEntity &other) const
	{
		return id_ != other.id_;
	}
	bool operator==(const IEntity *other) const
	{
		return id_ == other->id_;
	}
	bool operator!=(const IEntity *other) const
	{
		return id_ != other->id_;
	}

	virtual const EntityTypeId getEntityTypeId() const = 0;

	virtual void OnDisable()
	{
	}
	virtual void OnEnable()
	{
	}

protected:
	EntityId id_;
	bool active_;
};

#endif // VENGINE3D_IENTITY_H
