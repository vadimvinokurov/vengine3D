//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_SHAPE_H
#define VENGINE3D_VE_SHAPE_H

#include "Math/ve_vector.hpp"
#include "Math/ve_transform.hpp"

#include <vector>
#include <memory>

namespace VE {
    class Shape {
    public:
        Shape() {
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
        };

        const float *verticesGLFormatData() const  {
            return reinterpret_cast<const float *>(vertices_.data());
        }

        const void *indicesGLFormatData(unsigned int offset = 0) const {
            return static_cast<const void *>(indices_.data() + offset);
        }

        //virtual Vector farthestVertexInDirection(const Vector &direction) const = 0;
        virtual ~Shape() {};
    protected:
        std::vector<VE::Vector> vertices_;
        std::vector<unsigned int> indices_;
    };

    using ShapePtr = std::shared_ptr<Shape>;
}


#endif //VENGINE3D_VE_SHAPE_H
