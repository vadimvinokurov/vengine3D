//
// Created by boris on 11/24/2021.
//

#ifndef VENGINE3D_VE_SPHERE_COLLIDER_H
#define VENGINE3D_VE_SPHERE_COLLIDER_H

#include "ve_collider.h"

namespace VE {
    class SphereCollider : public Collider {
    public:
        SphereCollider(float radius, float mass, const Vector &localPosition);

        static ColliderPtr create(float radius = 1.0f, float mass = 1.0f, const Vector &localPosition = Vector());

        void setLocalPosition(const Vector& localPosition) override;
        Vector farthestVertexInDirection(const Vector &direction) const override;
        void setTransform(const Transform &transform) override;
        Vector getCenterOfMass() const override;
        Matrix3 getInertia() const override;
        float getRadius() const;


        virtual const void *verticesGLFormatData() const override;
        virtual const void *indicesGLFormatData(unsigned int offset = 0) const override;
        virtual unsigned int indecesSize() const override;
    private:
        void setGlvertices();
        Vector localCenter_;
        Vector globalCenter_;
        float radius_ = 1.0f;


        std::vector<Vector> glVerticesBuffer_;
        std::vector<unsigned int> glIndicesBuffer_;
    };
}


#endif //VENGINE3D_VE_SPHERE_COLLIDER_H
