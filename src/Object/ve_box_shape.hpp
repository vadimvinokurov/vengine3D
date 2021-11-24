//
// Created by boris on 11/24/2021.
//

#ifndef VENGINE3D_VE_BOX_SHAPE_HPP
#define VENGINE3D_VE_BOX_SHAPE_HPP

#include "ve_shape.h"

namespace VE{
    class BoxShape: public Shape {
    public:
        BoxShape();
        virtual Vector farthestVertexInDirection(const Vector &direction) const override;
    };
}



#endif //VENGINE3D_VE_BOX_SHAPE_HPP
