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
        ColliderType shapeType() const;
        virtual Vector farthestVertexInDirection(const Vector &direction) const = 0;
        virtual void setTransform(const Transform &transform);

        const Transform &transform() const;
        const Vector& centerOfMass() const;

        virtual void setLocalTransform(const Transform &localTransform);

        float mass() const;
        const Matrix33 &inertia() const;

        void setColor(const Color &color) const;
        Color color() const;


        virtual const void *verticesGLFormatData() const = 0;
        virtual const void *indicesGLFormatData(unsigned int offset = 0) const = 0;
        virtual unsigned int indecesSize() const = 0;

        virtual ~Collider();
    protected:
        explicit Collider(ColliderType shapeType);

        const ColliderType shapeType_;


        Transform transform_;
        float mass_;
        Matrix33 inertia_;
        Vector centerOfMass_;
        mutable VE::Color color_;
    };

    using ColliderPtr = std::shared_ptr<Collider>;
    using ConstColliderPtr = std::shared_ptr<const Collider>;
}


#endif //VENGINE3D_VE_COLLIDER_H
