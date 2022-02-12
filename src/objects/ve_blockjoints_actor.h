//
// Created by boris on 2/12/2022.
//

#ifndef VENGINE3D_VE_BLOCKJOINTS_ACTOR_H
#define VENGINE3D_VE_BLOCKJOINTS_ACTOR_H

#include "ve_colliders.h"
#include "ve_rigid_body.h"
#include "physics/ve_ball_socket_joint_solver.h"
#include "ve_utils.h"
#include "ve_actor.h"
#include "render/ve_debug_draw.h"

namespace VE {
    class BlockJoints : public Actor {
    public:
        BlockJoints(const Vector3 &position) {
            float blockSize = 0.4f;
            float offset = 0.1f;

            auto point = RigidBody::create(
                    {
                            create<SphereCollider>(0.05f, 0),
                    });
            point->setTransform(Transform(position));
            bodys_.push_back(point);


            for (int i = 0; i < 25; ++i) {
                auto body = RigidBody::create(
                        {
                                create<BoxCollider>(blockSize, 0.1, 0.1, 1),
                        });
                body->setTransform(Transform(Vector3((blockSize + offset) / 2 + (blockSize + offset) * i, 0, 0) + position));
                body->setGravity(Vector3(0, 0, -9.8f));
                bodys_.push_back(body);

                Vector3 jointPoint = Vector3((blockSize + offset) * i, 0, 0) + position;
                joints_.emplace_back(bodys_[i], bodys_[i + 1], jointPoint);
            }

        }
    };

    using RagdollActorPtr = std::shared_ptr<VE::RagdollActor>;

}
#endif //VENGINE3D_VE_BLOCKJOINTS_ACTOR_H
