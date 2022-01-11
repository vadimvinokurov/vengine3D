//
// Created by boris on 1/11/2022.
//

#ifndef VENGINE3D_VE_ACTOR_H
#define VENGINE3D_VE_ACTOR_H

#include "ve_colliders.h"
#include "ve_rigid_body.h"
#include "physics/ve_ball_socket_joint_solver.h"

namespace VE {

    class Actor;

    using ActorPtr = std::shared_ptr<VE::Actor>;

    class Actor {
    public:
        static ActorPtr create(const Vector &position = Vector()) {
            return std::make_shared<Actor>(position);
        }

        Actor(const Vector &position) {
            RigidBodyPtr body = RigidBody::create(
                    {
                            SphereCollider::create(0.2f, 5, Vector(0.0f, 0.0f, 0.8f)),

                            SphereCollider::create(0.3f, 15, Vector(0.0f, 0.0f, 0.3f)),
                            SphereCollider::create(0.3f, 15, Vector(0.0f, 0.0f, 0.1f)),
                            SphereCollider::create(0.3f, 15, Vector(0.0f, 0.0f, -0.1f)),
                            SphereCollider::create(0.3f, 15, Vector(0.0f, 0.0f, -0.3f)),
                    });
            body->setTransform(Transform(Vector(0, 0, 0) + position));
            bodys_.push_back(body);

            RigidBodyPtr rightHand = RigidBody::create(
                    {
                            SphereCollider::create(0.15f, 1, Vector(-0.3f, 0.0f, 0.0f)),
                            SphereCollider::create(0.15f, 1, Vector(-0.15f, 0.0f, 0.0f)),
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, 0.0f)),
                            SphereCollider::create(0.15f, 1, Vector(0.15f, 0.0f, 0.0f)),
                            SphereCollider::create(0.15f, 1, Vector(0.3f, 0.0f, 0.0f)),
                    });
            rightHand->setTransform(Transform(Vector(0.8f, 0, 0.35f) + position));
            bodys_.push_back(rightHand);

            RigidBodyPtr leftHand = RigidBody::create(
                    {
                            SphereCollider::create(0.15f, 1, Vector(-0.3f, 0.0f, 0.0f)),
                            SphereCollider::create(0.15f, 1, Vector(-0.15f, 0.0f, 0.0f)),
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, 0.0f)),
                            SphereCollider::create(0.15f, 1, Vector(0.15f, 0.0f, 0.0f)),
                            SphereCollider::create(0.15f, 1, Vector(0.3f, 0.0f, 0.0f)),
                    });
            leftHand->setTransform(Transform(Vector(-0.8f, 0, 0.35f) + position));
            bodys_.push_back(leftHand);

            RigidBodyPtr rightFood = RigidBody::create(
                    {
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, -0.3f)),
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, -0.15f)),
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, 0.0f)),
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, 0.15f)),
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, 0.3f)),

                            //BoxCollider::create(0.15f, 0.8f, 0.3f, 5, Vector(0.0f, 0.0f, 0.0f))
                    });
            rightFood->setTransform(Transform(Vector(0.2f, 0, -1.1f) + position));
            bodys_.push_back(rightFood);

            RigidBodyPtr leftFood = RigidBody::create(
                    {
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, -0.3f)),
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, -0.15f)),
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, 0.0f)),
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, 0.15f)),
                            SphereCollider::create(0.15f, 1, Vector(0.0f, 0.0f, 0.3f)),
                            //BoxCollider::create(0.15f, 0.8f, 0.3f, 5, Vector(0.0f, 0.0f, 0.0f))
                    });
            leftFood->setTransform(Transform(Vector(-0.2f, 0, -1.1f) + position));
            bodys_.push_back(leftFood);

            Vector pointRightHand(0.325f, 0, 0.35f);
            Vector pointLeftHand(-0.325f, 0, 0.35f);
            Vector pointRightFood(0.2f, 0, -0.625f);
            Vector pointLeftFood(-0.2f, 0, -0.625f);

            joints_.emplace_back(body, leftHand, pointLeftHand + position);
            joints_.emplace_back(body, rightHand, pointRightHand + position);
            joints_.emplace_back(body, leftFood, pointLeftFood + position);
            joints_.emplace_back(body, rightFood, pointRightFood + position);

            for (auto &o:bodys_) {
                o->setColor(Color(0.3f, 0.7f, 0.3f));
                o->setGravity(Vector(0.0f, 0.0f, -9.8f));
            }
        }

        void update(float dt) {
            for (size_t i = 0; i < globalParameters.iterations; i++) {
                for (auto &joint:joints_) {
                    joint.applyImpulse(dt);
                }
            }
        }

        std::vector<VE::RigidBodyPtr> &getObjects() {
            return bodys_;
        }

    private:
        std::vector<VE::RigidBodyPtr> bodys_;
        std::vector<VE::BallAndSocketJointSolver> joints_;
    };


}


#endif //VENGINE3D_VE_ACTOR_H
