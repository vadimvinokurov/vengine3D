//
// Created by boris on 11/24/2021.
//

#include "ve_sphere_shape.h"

using namespace VE;

SphereShape::SphereShape(Vector center) : SphereShape(1.0f, center) {

}

SphereShape::SphereShape(float radius, Vector center) : Shape(sphereShape),
                                                        center_(center),
                                                        radius_(radius) {

    int nT = 10;
    int nF = 100;

    float dT = M_PI / nT;
    float dF = M_PI * 2 / nF;
    for (int i = 0; i < nT; i++) {
        for (int j = 0; j < nF; j++) {
            vertices_.emplace_back(radius_ * sinf(dT * i) * cosf(dF * j),
                                   radius_ * sinf(dT * i) * sinf(dF * j),
                                   radius_ * cosf(dT * i));
        }
    }

}

Vector SphereShape::farthestVertexInDirection(const Vector &direction) const {
    return direction * radius_ + center_;
}


