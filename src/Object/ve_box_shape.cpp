//
// Created by boris on 11/24/2021.
//

#include "ve_box_shape.h"

using namespace VE;

BoxShape::BoxShape(VE::Vector position) : Shape(sphereShape) {
    BoxShape(1.0f, 1.0f, 1.0f, position);
}

BoxShape::BoxShape(float width, float height, float depth, VE::Vector position) : Shape(sphereShape) {

    vertices_ = {Vector(0, 0, 0) + position,
                 Vector(width, 0, 0) + position,
                 Vector(width, depth, 0) + position,
                 Vector(0, depth, 0) + position,
                 Vector(0, 0, height) + position,
                 Vector(width, 0, height) + position,
                 Vector(width, depth, height) + position,
                 Vector(0, depth, height) + position};

    indices_ = {
            3, 2, 1, 0,
            4, 5, 6, 7,
            0, 1, 5, 4,
            2, 3, 7, 6,
            3, 0, 4, 7,
            1, 2, 6, 5
    };

    Transform transform;



    for(Vector &vertex: vertices_){
        vertex = transform.applyTransform(vertex);
    }

}

Vector BoxShape::farthestVertexInDirection(const Vector &direction) const {
    return direction;
}



