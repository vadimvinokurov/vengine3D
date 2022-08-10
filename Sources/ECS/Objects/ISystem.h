//
// Created by boris on 8/9/2022.
//

#ifndef VENGINE3D_ISYSTEM_H
#define VENGINE3D_ISYSTEM_H

#include "VObject.h"
#include "ECS/Utils/IdManagers.h"

using SystemPriority = uint16;

class ISystem : public VObject
{
	friend class SystemManager;

public:
	static const SystemPriority LOWEST_SYSTEM_PRIORITY = std::numeric_limits<SystemPriority>::min();

	static const SystemPriority VERY_LOW_SYSTEM_PRIORITY = 99;
	static const SystemPriority LOW_SYSTEM_PRIORITY = 100;

	static const SystemPriority NORMAL_SYSTEM_PRIORITY = 200;

	static const SystemPriority MEDIUM_SYSTEM_PRIORITY = 300;

	static const SystemPriority HIGH_SYSTEM_PRIORITY = 400;
	static const SystemPriority VERY_HIGH_SYSTEM_PRIORITY = 401;

	static const SystemPriority HIGHEST_SYSTEM_PRIORITY = std::numeric_limits<SystemPriority>::max();

	ISystem(SystemPriority priority = NORMAL_SYSTEM_PRIORITY, float updateInterval = -1.0f)
		: priority_(priority), updateInterval_(updateInterval){};
	virtual ~ISystem() = default;

	virtual void preUpdate(float dt){};
	virtual void update(float dt){};
	virtual void postUpdate(float dt){};

	virtual SystemTypeId getSystemTypeId() const = 0;

	// private:
	SystemPriority priority_ = 0;
	float timeSinceLastUpdate_ = 0;
	float updateInterval_ = -1.0f;
	bool enabled_ = true;
	bool needsUpdate_ = true;
	SystemManager *systemManager_;
};

#endif // VENGINE3D_ISYSTEM_H
