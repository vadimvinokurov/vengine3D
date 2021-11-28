//
// Created by boris on 11/24/2021.
//

#include "ve_sphere_collider.h"

using namespace VE;

SphereShape::SphereShape(Vector center) : SphereShape(1.0f, center) {

}

SphereShape::SphereShape(float radius, Vector center) : Shape(sphereShape),
                                                        center_(center),
                                                        radius_(radius) {

    int nT = 10;
    int nF = 10;

    float dT = M_PI / nT;
    float dF = M_PI * 2 / nF;
    for (int i = 1; i < nT; i++) {
        for (int j = 0; j < nF; j++) {
            vertices_.emplace_back(radius_ * sinf(dT * i) * cosf(dF * j),
                                   radius_ * sinf(dT * i) * sinf(dF * j),
                                   radius_ * cosf(dT * i));

        }
    }

    for (int i = 1; i < nT - 1; i++) {
        for (int j = 0; j < nF - 1; j++) {
            indices_.push_back((i - 1) * nF + j);
            indices_.push_back((i - 1) * nF + j + 1);
            indices_.push_back(i * nF + j + 1);
            indices_.push_back(i * nF + j);
        }
        indices_.push_back((i - 1) * nF + nF - 1);
        indices_.push_back((i - 1) * nF + 0);
        indices_.push_back(i * nF + 0);
        indices_.push_back(i * nF + nF - 1);
    }


}

Vector SphereShape::farthestVertexInDirection(const Vector &direction) const {
    return direction * radius_ + center_;
}


