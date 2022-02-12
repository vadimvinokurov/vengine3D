//
// Created by boris on 2/12/2022.
//

#ifndef VENGINE3D_VE_RAGDOLL_ACTOR_H
#define VENGINE3D_VE_RAGDOLL_ACTOR_H

#include "ve_colliders.h"
#include "ve_rigid_body.h"
#include "physics/ve_ball_socket_joint_solver.h"
#include "ve_utils.h"
#include "ve_actor.h"

namespace VE {
    class RagdollActor: public Actor {
    public:
        RagdollActor(const Vector3 &position) {
            auto body = RigidBody::create(
                    {

                            create<SphereCollider>(0.2f, 5, Vector3(0.0f, 0.0f, 0.8f)),

                            create<SphereCollider>(0.3f, 20, Vector3(0.0f, 0.0f, 0.3f)),
                            create<SphereCollider>(0.3f, 20, Vector3(0.0f, 0.0f, 0.1f)),
                            create<SphereCollider>(0.3f, 20, Vector3(0.0f, 0.0f, -0.1f)),
                            create<SphereCollider>(0.3f, 20, Vector3(0.0f, 0.0f, -0.3f)),
                    });
            body->setTransform(Transform(Vector3(0, 0, 0) + position));
            bodys_.push_back(body);

            auto rightHand = RigidBody::create(
                    {
                            create<SphereCollider>(0.15f, 1, Vector3(-0.3f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(-0.15f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.15f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.3f, 0.0f, 0.0f)),
                    });
            rightHand->setTransform(Transform(Vector3(0.8f, 0, 0.35f) + position));
            bodys_.push_back(rightHand);

            auto leftHand = RigidBody::create(
                    {
                            create<SphereCollider>(0.15f, 1, Vector3(-0.3f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(-0.15f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.15f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.3f, 0.0f, 0.0f)),
                    });
            leftHand->setTransform(Transform(Vector3(-0.8f, 0, 0.35f) + position));
            bodys_.push_back(leftHand);

            auto rightFood = RigidBody::create(
                    {
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, -0.3f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, -0.15f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, 0.15f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, 0.3f)),

                            //BoxCollider::create(0.15f, 0.8f, 0.3f, 5, Vector(0.0f, 0.0f, 0.0f))
                    });
            rightFood->setTransform(Transform(Vector3(0.2f, 0, -1.1f) + position));
            bodys_.push_back(rightFood);

            auto leftFood = RigidBody::create(
                    {
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, -0.3f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, -0.15f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, 0.15f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, 0.3f)),
                            //BoxCollider::create(0.15f, 0.8f, 0.3f, 5, Vector(0.0f, 0.0f, 0.0f))
                    });
            leftFood->setTransform(Transform(Vector3(-0.2f, 0, -1.1f) + position));
            bodys_.push_back(leftFood);

            Vector3 pointRightHand(0.4f, 0, 0.35f);
            Vector3 pointLeftHand(-0.4f, 0, 0.35f);
            Vector3 pointRightFood(0.2f, 0, -0.625f);
            Vector3 pointLeftFood(-0.2f, 0, -0.625f);

            joints_.emplace_back(body, leftHand, pointLeftHand + position);
            joints_.emplace_back(body, rightHand, pointRightHand + position);
            joints_.emplace_back(body, leftFood, pointLeftFood + position);
            joints_.emplace_back(body, rightFood, pointRightFood + position);

            for (auto &o:bodys_) {
                o->setColor(Color(0.3f, 0.7f, 0.3f));
                o->setGravity(Vector3(0.0f, 0.0f, -9.8f));
            }
        }
    };
    using RagdollActorPtr = std::shared_ptr<VE::RagdollActor>;

}
#endif //VENGINE3D_VE_RAGDOLL_ACTOR_H
