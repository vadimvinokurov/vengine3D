//
// Created by boris on 12/7/2021.
//

#ifndef VENGINE3D_VE_CONTACT_POINTS_H
#define VENGINE3D_VE_CONTACT_POINTS_H

#include "objects/ve_box_collider.h"

namespace VE {
    void getContactPoints(const BoxCollider &collider1, const BoxCollider &collider2, const Vector &contactNormal);
}


#endif //VENGINE3D_VE_CONTACT_POINTS_H
