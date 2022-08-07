//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_IENTITY_H
#define VENGINE3D_IENTITY_H

#include "VObject.h"
#include "ECS/Utils/IdManagers.h"

using EntityId = idtype;
using EntityTypeId = idtype;

class IEntity : public VObject
{
	friend class EntityManager;

public:
	IEntity() : id_(INVALID_ID), active_(true){};
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

	virtual EntityTypeId getEntityTypeId() const = 0;

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
