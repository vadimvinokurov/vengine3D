//
// Created by boris on 1/11/2022.
//

#ifndef VENGINE3D_VE_ACTOR_H
#define VENGINE3D_VE_ACTOR_H

#include "ve_colliders.h"
#include "ve_rigid_body.h"
#include "physics/ve_ball_socket_joint_solver.h"
#include "ve_utils.h"

namespace VE {

    class Actor;

    using ActorPtr = std::shared_ptr<VE::Actor>;

    class Actor {
    public:
        Actor(const Vector3 &position) {
            RigidBodyPtr body = RigidBody::create(
                    {

                            create<SphereCollider>(0.2f, 5, Vector3(0.0f, 0.0f, 0.8f)),

                            create<SphereCollider>(0.3f, 20, Vector3(0.0f, 0.0f, 0.3f)),
                            create<SphereCollider>(0.3f, 20, Vector3(0.0f, 0.0f, 0.1f)),
                            create<SphereCollider>(0.3f, 20, Vector3(0.0f, 0.0f, -0.1f)),
                            create<SphereCollider>(0.3f, 20, Vector3(0.0f, 0.0f, -0.3f)),
                    });
            body->setTransform(Transform(Vector3(0, 0, 0) + position));
            bodys_.push_back(body);

            RigidBodyPtr rightHand = RigidBody::create(
                    {
                            create<SphereCollider>(0.15f, 1, Vector3(-0.3f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(-0.15f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.15f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.3f, 0.0f, 0.0f)),
                    });
            rightHand->setTransform(Transform(Vector3(0.8f, 0, 0.35f) + position));
            bodys_.push_back(rightHand);

            RigidBodyPtr leftHand = RigidBody::create(
                    {
                            create<SphereCollider>(0.15f, 1, Vector3(-0.3f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(-0.15f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.0f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.15f, 0.0f, 0.0f)),
                            create<SphereCollider>(0.15f, 1, Vector3(0.3f, 0.0f, 0.0f)),
                    });
            leftHand->setTransform(Transform(Vector3(-0.8f, 0, 0.35f) + position));
            bodys_.push_back(leftHand);

            RigidBodyPtr rightFood = RigidBody::create(
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

            RigidBodyPtr leftFood = RigidBody::create(
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

        void update(float dt) {
            for (auto &joint:joints_) {
                joint.applyImpulse(dt);
            }
        }

        std::vector<VE::RigidBodyPtr> &getObjects() {
            return bodys_;
        }

        std::vector<VE::BallAndSocketJointSolver> &getJoints() {
            return joints_;
        }

    private:
        std::vector<VE::RigidBodyPtr> bodys_;
        std::vector<VE::BallAndSocketJointSolver> joints_;
    };


}


#endif //VENGINE3D_VE_ACTOR_H
