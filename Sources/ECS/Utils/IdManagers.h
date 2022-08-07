//
// Created by boris on 8/6/2022.
//

#ifndef VENGINE3D_IDMANAGERS_H
#define VENGINE3D_IDMANAGERS_H

#include "ObjectIdManager.h"
#include "TypeIdManage.h"

static constexpr idtype INVALID_ID = std::numeric_limits<idtype>::max();

using EntityId = idtype;
using EntityTypeId = idtype;

using ComponentId = idtype;
using ComponentTypeId = idtype;

#endif // VENGINE3D_IDMANAGERS_H
