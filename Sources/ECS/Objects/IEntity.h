//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_IENTITY_H
#define VENGINE3D_IENTITY_H

#include "VObject.h"
#include "ECS/Utils/IdManagers.h"
#include "ECS/ComponentManager.h"

class IEntity : public VObject
{
	friend class EntityManager;

public:
	IEntity() : entityId_(INVALID_ID), active_(true){};
	virtual ~IEntity() = default;

	EntityId getEntityId() const
	{
		return entityId_;
	}

	template <typename T, typename... Args>
	void addComponent(Args &&...args)
	{
		componentManage_->addComponent<T>(entityId_, std::forward<Args>(args)...);
	}

	template <typename T>
	void getComponent()
	{
		componentManage_->getComponent<T>(entityId_);
	}

	template <typename T>
	void removeComponent()
	{
		componentManage_->removeComponent<T>(entityId_);
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
		return entityId_ == other.entityId_;
	}
	bool operator!=(const IEntity &other) const
	{
		return entityId_ != other.entityId_;
	}
	bool operator==(const IEntity *other) const
	{
		return entityId_ == other->entityId_;
	}
	bool operator!=(const IEntity *other) const
	{
		return entityId_ != other->entityId_;
	}

	virtual EntityTypeId getEntityTypeId() const = 0;

	virtual void OnDisable()
	{
	}
	virtual void OnEnable()
	{
	}

protected:
	EntityId entityId_;
	bool active_;
	ComponentManager *componentManage_;
};

#endif // VENGINE3D_IENTITY_H
