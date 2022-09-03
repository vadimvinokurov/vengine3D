//
// Created by boris on 8/23/2022.
//

#ifndef VENGINE3D_BONE_H
#define VENGINE3D_BONE_H

#include "EnginePlatform.h"
#include "Transform.h"

static constexpr int32 INVALID_BODE_ID = -1;

struct Bone{
	std::string name;
	int32 parentId;
	Transform transform;
};

#endif // VENGINE3D_BONE_H
