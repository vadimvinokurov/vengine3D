//
// Created by boris on 11/28/2021.
//

#ifndef VENGINE3D_VE_GJK_H
#define VENGINE3D_VE_GJK_H
#include "Object/ve_collider.h"

namespace VE{
    bool gjk(const Collider &collider1, const Collider &collider2, VE::Vector &penetrationVector);
}



#endif //VENGINE3D_VE_GJK_H
