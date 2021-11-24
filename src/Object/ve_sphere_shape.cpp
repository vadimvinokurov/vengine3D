//
// Created by boris on 11/24/2021.
//

#include "ve_sphere_shape.h"

VE::SphereShape::SphereShape(float radius, float center) : Shape(sphereShape) {
}

VE::Vector VE::SphereShape::farthestVertexInDirection(const VE::Vector &direction) const {
    return VE::Vector();
}
