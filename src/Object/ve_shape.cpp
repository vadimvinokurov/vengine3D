//
// Created by boris on 11/18/2021.
//

#include "ve_shape.h"

using namespace VE;

VE::Shape::Shape() {

}

const float *Shape::verticesGLFormatData() const {
    return reinterpret_cast<const float *>(vertices_.data());
}

const void *Shape::indicesGLFormatData(unsigned int offset) const {
    return static_cast<const void *>(indices_.data() + offset);
}

Shape::~Shape() {

}
