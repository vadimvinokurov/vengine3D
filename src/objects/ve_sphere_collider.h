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

        static std::vector<Vector3> getGetRenderVertices(float radius, const Vector3 &position);
        static std::vector<unsigned int> getGetRenderIndices();

    private:
        Vector3 localCenter_;
        Vector3 globalCenter_;
        float radius_ = 1.0f;

        static constexpr int nT = 10;
        static constexpr int nF = 10;
    };
}


#endif //VENGINE3D_VE_SPHERE_COLLIDER_H
