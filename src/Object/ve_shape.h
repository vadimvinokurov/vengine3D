//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_SHAPE_H
#define VENGINE3D_VE_SHAPE_H

#include "Math/ve_vector.hpp"

#include <vector>
#include <memory>

namespace VE {
    class Shape {
    public:
        Shape();

        virtual ~Shape();
    private:
        std::vector<VE::Vector> vertices;
    };

    using ShapePtr = std::shared_ptr<Shape>;
}


#endif //VENGINE3D_VE_SHAPE_H
