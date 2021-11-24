//
// Created by boris on 11/24/2021.
//

#include "ve_box_shape.h"

using namespace VE;

BoxShape::BoxShape() {
    vertices_ = {Vector(0, 0, 0),
                 Vector(1, 0, 0),
                 Vector(1, 1, 0),
                 Vector(0, 1, 0),
                 Vector(0, 0, 1),
                 Vector(1, 0, 1),
                 Vector(1, 1, 1),
                 Vector(0, 1, 1)};

    indices_ = {
            3, 2, 1, 0,
            4, 5, 6, 7,
            0, 1, 5, 4,
            2, 3, 7, 6,
            3, 0, 4, 7,
            1, 2, 6, 5
    };
}

Vector BoxShape::farthestVertexInDirection(const Vector &direction) const {
    return direction;
}
