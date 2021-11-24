//
// Created by boris on 11/24/2021.
//

#ifndef VENGINE3D_VE_BOX_SHAPE_H
#define VENGINE3D_VE_BOX_SHAPE_H

#include "ve_shape.h"

namespace VE {
    class BoxShape : public Shape {
    public:
        BoxShape(float width = 1.0f, float height = 1.0f, float depth = 1.0f, VE::Vector position = Vector(0, 0, 0));
        BoxShape(VE::Vector position);
        virtual Vector farthestVertexInDirection(const Vector &direction) const override;
    };
}


#endif //VENGINE3D_VE_BOX_SHAPE_H
