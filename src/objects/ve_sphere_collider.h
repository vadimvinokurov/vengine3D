//
// Created by boris on 11/24/2021.
//

#ifndef VENGINE3D_VE_SPHERE_COLLIDER_H
#define VENGINE3D_VE_SPHERE_COLLIDER_H

#include "ve_collider.h"

namespace VE {
    class SphereCollider : public Collider {
    public:
        SphereCollider(float radius, float mass, const Vector3 &localPosition);

        static ColliderPtr create(float radius = 1.0f, float mass = 1.0f, const Vector3 &localPosition = Vector3());

        void setLocalPosition(const Vector3& localPosition) override;
        Vector3 farthestVertexInDirection(const Vector3 &direction) const override;
        void setTransform(const Transform &transform) override;
        Vector3 getCenterOfMass() const override;
        Matrix3 getInertia() const override;
        float getRadius() const;


        virtual const std::vector<Vector3>& vertices() const override;
        virtual const std::vector<unsigned int>& indices() const override;
    private:
        void setGlvertices();
        Vector3 localCenter_;
        Vector3 globalCenter_;
        float radius_ = 1.0f;


        std::vector<Vector3> glVerticesBuffer_;
        std::vector<unsigned int> glIndicesBuffer_;
    };
}


#endif //VENGINE3D_VE_SPHERE_COLLIDER_H
