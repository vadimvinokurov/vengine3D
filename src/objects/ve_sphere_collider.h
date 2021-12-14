//
// Created by boris on 11/24/2021.
//

#ifndef VENGINE3D_VE_SPHERE_COLLIDER_H
#define VENGINE3D_VE_SPHERE_COLLIDER_H

#include "ve_collider.h"

namespace VE {
    class SphereCollider : public Collider {
    public:
        SphereCollider(Vector center);
        SphereCollider(float radius = 1.0f, const Vector &center = Vector(), float mass = 1.0f);


        virtual Vector farthestVertexInDirection(const Vector &direction) const override;
        virtual void setTransform(const Transform &transform) override;
        virtual void setLocalTransform(const Transform &localTransform) override;
        virtual const Vector getCenterOfMass() const override;


        virtual const void *verticesGLFormatData() const override;
        virtual const void *indicesGLFormatData(unsigned int offset = 0) const override;
        virtual unsigned int indecesSize() const override;
    private:
        void computeSphereInertia();
        void setGlvertices();
        Vector localCenter_;
        Vector globalCenter_;
        float radius_ = 1.0f;


        std::vector<Vector> glVerticesBuffer_;
        std::vector<unsigned int> glIndicesBuffer_;
    };
}


#endif //VENGINE3D_VE_SPHERE_COLLIDER_H