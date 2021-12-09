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
        SphereCollider(float radius = 1.0f, Vector center = Vector(), float mass = 1.0f);
        virtual Vector farthestVertexInDirection(const Vector &direction) const override;
    private:
        void computeSphereInertia();
        void setGlvertices();
        float radius_ = 1.0f;
    };
}


#endif //VENGINE3D_VE_SPHERE_COLLIDER_H
