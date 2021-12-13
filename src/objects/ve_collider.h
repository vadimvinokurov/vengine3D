//
// Created by boris on 11/18/2021.
//

#ifndef VENGINE3D_VE_COLLIDER_H
#define VENGINE3D_VE_COLLIDER_H

#include "math/ve_vector.h"
#include "math/ve_transform.h"
#include "math/ve_matrix33.h"

#include <vector>
#include <memory>

namespace VE {
    enum class ColliderType {
        sphere = 0, box, capsule
    };

    class Collider {
    public:
        ColliderType shapeType() const { return shapeType_; }

        float mass() const { return mass_; }

        const Matrix33 &inertia() const { return inertia_; }

        void setColor(const Color &color) const { color_ = color; }

        Color color() const { return color_; }

        virtual const Vector getCenterOfMass() const = 0;
        virtual void setLocalTransform(const Transform &localTransform) = 0;
        virtual Vector farthestVertexInDirection(const Vector &direction) const = 0;
        virtual void setTransform(const Transform &transform) = 0;

        virtual const void *verticesGLFormatData() const = 0;
        virtual const void *indicesGLFormatData(unsigned int offset = 0) const = 0;
        virtual unsigned int indecesSize() const = 0;

        virtual ~Collider() {}

    protected:
        explicit Collider(ColliderType shapeType) : shapeType_(shapeType) {}

        Matrix33 inertia_;
        float mass_;
        const ColliderType shapeType_;

        mutable Color color_ = Color(0.5f, 0.5f, 0.5f);
    };

    using ColliderPtr = std::shared_ptr<Collider>;
    using ConstColliderPtr = std::shared_ptr<const Collider>;
}


#endif //VENGINE3D_VE_COLLIDER_H
