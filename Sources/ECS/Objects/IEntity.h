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
	IEntity() = default;
	virtual ~IEntity() = default;

	EntityId getEntityId() const
	{
		return id_;
	}

	template <typename T, typename... Args>
	T *addComponent(Args &&...args)
	{
		return componentManage_->addComponent<T>(id_, std::forward<Args>(args)...);
	}

	template <typename T>
	T *getComponent()
	{
		return componentManage_->getComponent<T>(id_);
	}

	template <typename T>
	void removeComponent()
	{
		componentManage_->removeComponent<T>(id_);
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
	EntityId id_ = INVALID_ID;
	bool active_ = true;
	ComponentManager *componentManage_ = nullptr;
};

#endif // VENGINE3D_IENTITY_H
