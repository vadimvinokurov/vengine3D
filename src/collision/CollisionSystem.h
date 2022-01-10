//
// Created by boris on 1/10/2022.
//

#ifndef VENGINE3D_COLLISIONSYSTEM_H
#define VENGINE3D_COLLISIONSYSTEM_H

#include "objects/colliders.h"

namespace VE {
    class CollisionSystem {
    public:
        virtual bool testIntersection() = 0;
        virtual Vector getPenetrationVector() = 0;
    };

    class BoxBoxCollision : public CollisionSystem {
    public:
        BoxBoxCollision(const BoxCollider &box1, const BoxCollider &box2) :
                box1_(box1), box2_(box2) {}

        bool testIntersection() override {
            return true;
        }

        Vector getPenetrationVector() override {
            return Vector();
        }

    private:
        const BoxCollider &box1_;
        const BoxCollider &box2_;
    };

    class SphereSphereCollision : public CollisionSystem {
    public:
        SphereSphereCollision(const SphereCollider &sphere1, const SphereCollider &sphere2) :
                sphere1_(sphere1), sphere2_(sphere2) {}

        bool testIntersection() override {
            return true;
        }

        Vector getPenetrationVector() override {
            return Vector();
        }

    private:
        const SphereCollider &sphere1_;
        const SphereCollider &sphere2_;
    };

    class BoxSphereCollision : public CollisionSystem {
    public:
        BoxBoxCollision(const BoxCollider &box1, const BoxCollider &box2) :
                box1_(box1), box2_(box2) {}

        bool testIntersection() override {
            return true;
        }

        Vector getPenetrationVector() override {
            return Vector();
        }

    private:
        const BoxCollider &box1_;
        const BoxCollider &box2_;
    };
}


#endif //VENGINE3D_COLLISIONSYSTEM_H
