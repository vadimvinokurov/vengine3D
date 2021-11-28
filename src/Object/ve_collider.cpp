//
// Created by boris on 11/18/2021.
//

#include "ve_collider.h"

using namespace VE;

VE::Shape::Shape(ShapeType shapeType) : shapeType_(shapeType) {

}

const float *Shape::verticesGLFormatData() const {
    return reinterpret_cast<const float *>(vertices_.data());
}

const void *Shape::indicesGLFormatData(unsigned int offset) const {
    return static_cast<const void *>(indices_.data() + offset);
}

Shape::~Shape() {

}

ShapeType Shape::shapeType() const {
    return shapeType_;
}

unsigned int Shape::indecesSize() const {
    return indices_.size();
}