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

        float mass() const;
        const Matrix33 &inertia() const;

        void setColor(const Color &color) const;
        Color color() const;


        const float *verticesGLFormatData() const;
        unsigned int indecesSize() const;
        const void *indicesGLFormatData(unsigned int offset = 0) const;
        const std::vector<Vector> &vertices() const;
        virtual ~Collider();
    protected:
        explicit Collider(ColliderType shapeType);

        const ColliderType shapeType_;

        Transform transform_;
        Matrix33 inertia_;
        float mass_;
        Vector centerOfMass_;

        mutable VE::Color color_;
        std::vector<Vector> glvertices_;
        std::vector<unsigned int> glindices_;
    };

    using ColliderPtr = std::shared_ptr<Collider>;
    using ConstColliderPtr = std::shared_ptr<const Collider>;
}


#endif //VENGINE3D_VE_COLLIDER_H
