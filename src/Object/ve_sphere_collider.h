//
// Created by boris on 11/24/2021.
//

#ifndef VENGINE3D_VE_SPHERE_COLLIDER_H
#define VENGINE3D_VE_SPHERE_COLLIDER_H

#include "ve_collider.h"

namespace VE {
    class SphereShape : public Shape {
    public:
        SphereShape(Vector center);
        SphereShape(float radius = 1.0f, Vector center = Vector());
        virtual Vector farthestVertexInDirection(const Vector &direction) const override;
    private:
        float radius_ = 1.0f;
        Vector center_;
    };
}


#endif //VENGINE3D_VE_SPHERE_COLLIDER_H
