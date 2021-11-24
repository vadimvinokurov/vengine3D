//
// Created by boris on 11/24/2021.
//

#ifndef VENGINE3D_VE_SPHERE_SHAPE_H
#define VENGINE3D_VE_SPHERE_SHAPE_H

#include "ve_shape.h"

namespace VE{
    class SphereShape: public Shape {
        SphereShape( float radius = 1.0f, float center = 1.0f);
        virtual Vector farthestVertexInDirection(const Vector &direction) const override;
    };
}



#endif //VENGINE3D_VE_SPHERE_SHAPE_H
